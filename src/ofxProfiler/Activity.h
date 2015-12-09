#pragma once

#include <string>
#include <unordered_map>
#include <deque>

#include <chrono>
#include <ctime>
#include <ratio>

#define OFXPROFILER_TEXT_WIDTH 20
#define OFXPROFILER_BAR_WIDTH 20

// The general pattern is :
//		Activity performs all profiling
//		NullActivity inherits all functions, and does nothing

namespace ofxProfiler {
	using namespace std;
	using namespace std::chrono;

	class Activity {
		static Activity * root;
		static Activity * current;
		static deque<Activity*> * history;
	public:
		static Activity & Root();
		static Activity *& Current();
		static deque<Activity*> & History();

		Activity(string name = "");
		~Activity();

		void begin();
		void end();
		void clear(); //doesn't clear the name, but deletes all children
		void clearDuration(); //clear duration and in all children

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
		
		high_resolution_clock::time_point beginTime;
		double duration; //seconds

		unordered_map<string, Activity*> subActivities;
	};
}