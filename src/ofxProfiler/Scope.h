#pragma once
#include "Activity.h"

namespace ofxProfiler {
	class Scope {
	public:
		Scope(const string & activityName);
		Scope(Activity & activity);
		~Scope();
	protected:
		Activity * activity;
	};
}