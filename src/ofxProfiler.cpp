#include "ofxProfiler.h"

namespace ofxProfiler {
	void begin(string activityName) {
		const auto currentActivity = ofxProfiler::Activity::Current();
		(*currentActivity)[activityName].begin();
	}

	void end() {
		(*ofxProfiler::Activity::Current()).end();
	}

	string getResults() {
		return ofxProfiler::Activity::Root().getResults();
	}
}