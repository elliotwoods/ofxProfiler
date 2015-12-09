!(https://raw.githubusercontent.com/elliotwoods/ofxProfiler/master/ofxaddons_thumbnail.png)

# Usage

## Tracking profiling

### Simple tracking

```c++
PROFILE_BEGIN("My activity");

doSomething();

PROFILE_END();
```

### Hierarchical tracking

To get a tree of tracked results (sub activities belong to larger activities), use the pattern:

```c++
PROFILE_BEGIN("My grouped activity");

PROFILE_BEGIN("Activity 1");
doSomething1();
PROFILE_END();

PROFILE_BEGIN("Activity 2");
doSomething1();
PROFILE_END();

PROFILE_BEGIN("Activity 3");
doSomething1();
PROFILE_END();

PROFILE_END();
```

### Scoped tracking

If you think that `PROFILE_BEGIN` and `PROFILE_END` are messy, you can also used scoped profiler pattern:

```c++
void update() {
	PROFILE_SCOPE("update");

	{
		PROFILE_SCOPE("Activity 1");
		doSomething1();
	}

	{
		PROFILE_SCOPE("Activity 2");
		doSomething2();
	}
}
```

This works by creating temporary variables which are destroyed when the `{ }` scope ends. We call `begin`/`end` in the constructor/destructor respectively.

## Printing profile results

```c++
//prints a table of results for all profiled activities
cout << ofxProfiler::getResults();
```

## Switching profiling on/off

To switch off profiling, simply define `PROFILER_DISABLED` before including `ofxProfiler.h`.
This makes all the macros used for profiling equal to whitespace.

### ON

```c++
#include "ofxProfiler.h"
```

### OFF

```c++
#define PROFILER_DISABLED
#include "ofxProfiler.h"
```

# Notes

* Activities are kept in a tree
* There is a root activity (`ofxProfiler::Activity::Root()`). We don't call `begin()` and `end()` on this, we just use it as a collection
