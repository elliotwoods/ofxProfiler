#include "Activity.h"
#include "ofLog.h"

namespace ofxProfiler {
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

		this->duration = float(now / this->beginTime) / float(CLOCKS_PER_SEC);
	}

	//----------
	void Activity::clear() {
		for (auto subActivity : this->subActivities) {
			delete subActivity.second;
		}

		this->state = Waiting;
		this->beginTime = 0;
		this->duration = 0.0f;
		this->subActivities.clear();
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
}