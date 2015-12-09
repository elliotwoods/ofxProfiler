#pragma once

#include <string>
#include <map>
#include <time.h>

// The general pattern is :
//		Activity performs all profiling
//		NullActivity inherits all functions, and does nothing

namespace ofxProfiler {
	using namespace std;

	class Activity {
	public:
		Activity(string name = "");
		virtual ~Activity();

		virtual void begin();
		virtual void end();
		virtual void clear(); //doesn't clear the name

		const string & getName() const;

		virtual Activity & operator[](const string &);
	protected:
		string name;
		enum {
			Waiting,
			Active
		} state;
		
		clock_t beginTime;
		float duration;

		map<string, Activity*> subActivities;
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
}