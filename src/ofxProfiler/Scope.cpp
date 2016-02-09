#include "Scope.h"

namespace ofxProfiler {
	//----------
	Scope::Scope(const string & activityName) {
		this->activity = &(*Activity::Current())[activityName];
		this->activity->begin();
	}

	//----------
	Scope::Scope(Activity & activity) {
		this->activity = &activity;
		this->activity->begin();
	}

	//----------
	Scope::~Scope() {
		this->activity->end();
	}
}