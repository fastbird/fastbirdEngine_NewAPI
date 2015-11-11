#pragma warning (disable : 4251)

#define FBDLLProject
#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define LEAN_AND_MEAN
#include <Windows.h>
#else
#endif

#include <stack>
#include <map>
#include <queue>
#include <vector>
#include <assert.h>
#include <memory>
#include <bitset>

#include "FBDebugLib\Logger.h"
#include "FBTimerLib\Timer.h"