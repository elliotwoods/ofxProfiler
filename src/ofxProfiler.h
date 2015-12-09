#pragma once

#include "ofxProfiler/Activity.h"

namespace ofxProfiler {
	#ifdef PROFILER_DISABLE
		typedef ofxProfiler::NullActivity ActivitySwitched
	#else
		typedef ofxProfiler::Activity ActivitySwitched;
	#endif
}

extern ofxProfiler::ActivitySwitched profile;