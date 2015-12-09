#pragma once

#include "ofxProfiler/Activity.h"
#include "ofxProfiler/Scope.h"

namespace ofxProfiler {
	void startFrame();
	void begin(string activityName);
	void end();
	string getResults();
}

#ifdef PROFILER_DISABLED
	#define PROFILE_START_FRAME

	#define PROFILE_BEGIN(X) 

	#define PROFILE_END() 

	#define PROFILE_SCOPE(X)
#else 
	#define PROFILE_START_FRAME ofxProfiler::startFrame()

	#define PROFILE_BEGIN(X) ofxProfiler::begin(X)

	#define PROFILE_END() ofxProfiler::end()

	#define PROFILE_SCOPE(X) auto scopedProfiler = ofxProfiler::Scope(X)
#endif

