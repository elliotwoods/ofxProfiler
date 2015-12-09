#include "Activity.h"
#include "ofLog.h"
#include "ofUtils.h"

namespace ofxProfiler {
	//----------
	Activity * Activity::root = nullptr;
	Activity * Activity::current = nullptr;
	deque<Activity*> * Activity::history = nullptr;

	//----------
	Activity & Activity::Root() {
		if (!Activity::root) {
			Activity::root = new Activity("Root");
		}
		return *Activity::root;
	}

	//----------
	Activity *& Activity::Current() {
		if (!Activity::current) {
			Activity::current = &Activity::Root();
		}
		return Activity::current;
	}

	//----------
	deque<Activity*> & Activity::History() {
		if (!Activity::history) {
			Activity::history = new deque <Activity *>();
		}
		return * Activity::history;
	}

	//----------
	Activity::Activity(string name) {
		this->name = name;
		this->state = Waiting;
		this->beginTime = 0;
		this->duration = 0.0f;
	}

	//----------
	Activity::~Activity() {
		this->clear();
	}

	//----------
	void Activity::begin() {
		if (this->state == Active) {
			ofLogWarning("ofxProfiler::Activity::begin") << "Cannot begin activity [" << this->name << "] as it is already active.";
			return;
		}
		
		Activity::History().push_back(Activity::Current());
		Activity::current = this;
		this->state = Active;

		this->beginTime = clock();
	}

	//----------
	void Activity::end() {
		if (this->state == Waiting) {
			ofLogWarning("ofxProfiler::Activity::end") << "Cannot end activity [" << this->name << "] as it is not active.";
			return;
		}
		const auto now = clock();
		if (now < this->beginTime) {
			ofLogError("ofxProfiler::Activity::end") << "Activity ended before it began";
			return;
		}

		this->duration = float(now - this->beginTime) / float(CLOCKS_PER_SEC);

		Activity::Current() = Activity::History().back();
		Activity::history->pop_back();
		this->state = Waiting;
	}

	//----------
	void Activity::clear() {
		for (auto subActivity : this->subActivities) {
			delete subActivity.second;
		}
		this->subActivities.clear();

		this->state = Waiting;
		this->beginTime = 0;
		this->duration = 0.0f;
	}

	//----------
	const string & Activity::getName() const {
		return this->name;
	}

	//----------
	Activity & Activity::operator[](const string & name) {
		auto findActivity = this->subActivities.find(name);
		if (findActivity == this->subActivities.end()) {
			auto newActivity = new Activity(name);
			auto newActivityInserter = pair<string, Activity*>(name, newActivity);
			this->subActivities.insert(newActivityInserter);
			return * newActivity;
		}
		else {
			return * findActivity->second;
		}
	}

	//----------
	int Activity::getSubActivityGenerationCount() const {
		if (this->subActivities.empty()) {
			return 0;
		}

		int maxCount = 0;
		for (auto subActivity : this->subActivities) {
			auto count = subActivity.second->getSubActivityGenerationCount();
			if (count > maxCount) {
				maxCount = count;
			}
		}
		return maxCount + 1;
	}

	/*
	update.............[==============] 100%  30ms
	|-generation1......[=========     ] 50%   20ms
	  |-generation2....[===           ] 10%   5ms
	*/
	//----------
	void Activity::printGeneration(ostream & os, int generation, int textWidth, float totalTime) const {
		size_t length = 0;

		//--
		//Print tree
		//--
		//
		for (int i = 0; i < generation - 1; i++) {
			os << "  ";
			length += 2;
		}
		if (generation > 0) {
			os << "|-";
			length += 2;
		}
		//
		//--
		

		
		//--
		//Print name and '.'s
		//--
		//
		{
			auto nameLength = this->name.length();
			auto newLength = length + nameLength;

			//check if we need to crop the name to fit
			if (newLength <= textWidth) {
				os << this->name;
				length += nameLength;

				//pad with '.' until length = textWidth
				while (length < textWidth) {
					os << ".";
					length++;
				}
			}
			else {
				//crop the name (this should never be negative because we shouldn't call printGeneration with a bad length)
				os << this->name.substr(0, textWidth - length);
				length = textWidth;
			}
		}
		//
		//--



		//--
		//Print bar and %
		//--
		//
		{
			float fraction;
			if (this->duration == 0) {
				fraction = 0;
			}
			else {
				fraction = this->duration / totalTime;
			}

			int barLength = round(fraction * float(OFXPROFILER_BAR_WIDTH));
			os << "[";
			for (int i = 0; i < OFXPROFILER_BAR_WIDTH; i++) {
				if (i < barLength) {
					os << "=";
				}
				else {
					os << " ";
				}
			}
			os << "] ";

			string percentageString = ofToString(fraction * 100.0f, 1);
			for (auto i = percentageString.length(); i < 5; i++) {
				os << " ";
			}
			os << percentageString << "%";
		}
		//
		//--


		//--
		//Print time
		//--
		//
		os << " " << (this->duration * 1000.0f) << "ms";
		//
		//--

		os << endl;

		for (auto subActivity : this->subActivities) {
			subActivity.second->printGeneration(os, generation + 1, textWidth, totalTime);
		}
	}

	//----------
	string Activity::getResults() {
		if (this == & Activity::Root()) {
			stringstream ss;
			for (auto activity : subActivities) {
				ss << activity.second->getResults();
				ss << endl;
			}
			return ss.str();
		}
		else {
			if (this->state == Active) {
				ofLogError("ofxProfiler::Activity::getResults()") << "[" << this->name << "] Cannot print results whilst active. Please call end before getResults";
				return "";
			}
			auto generationCount = this->getSubActivityGenerationCount() + 1;

			stringstream ss;
			this->printGeneration(ss, 0, generationCount * 2 + OFXPROFILER_TEXT_WIDTH, this->duration);

			return ss.str();
		}
	}
}