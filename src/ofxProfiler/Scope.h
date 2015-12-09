#pragma once
#include "Activity.h"

namespace ofxProfiler {
	class Scope {
	public:
		Scope(const string & activityName);
		~Scope();
	protected:
		Activity * activity;
	};
}