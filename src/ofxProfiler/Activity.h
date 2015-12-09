#pragma once

#include <string>
#include <unordered_map>
#include <deque>
#include <time.h>

#define OFXPROFILER_TEXT_WIDTH 20
#define OFXPROFILER_BAR_WIDTH 20

// The general pattern is :
//		Activity performs all profiling
//		NullActivity inherits all functions, and does nothing

namespace ofxProfiler {
	using namespace std;

	class Activity {
		static Activity * root;
		static Activity * current;
		static deque<Activity*> * history;
	public:
		static Activity & Root();
		static Activity *& Current();
		static deque<Activity*> & History();

		Activity(string name = "");
		virtual ~Activity();

		virtual void begin();
		virtual void end();
		virtual void clear(); //doesn't clear the name

		const string & getName() const;

		virtual Activity & operator[](const string &);

		int getSubActivityGenerationCount() const;
		void printGeneration(ostream &, int generation, int textWidth, float totalTime) const;

		string getResults();
	protected:
		string name;
		enum {
			Waiting,
			Active
		} state;
		
		clock_t beginTime;
		float duration;

		unordered_map<string, Activity*> subActivities;
	};

	class NullActivity : public Activity {
	public:
		void begin() override { }
		void end() override { }
		void clear() override { }
		Activity & operator[](const string &) override {
			return * this;
		}
	};


#ifdef PROFILER_DISABLE
	typedef ofxProfiler::NullActivity ActivitySwitched
#else
	typedef ofxProfiler::Activity ActivitySwitched;
#endif
}