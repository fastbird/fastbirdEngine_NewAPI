#include "platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define FBRendererDLL __declspec(dllexport)
#else
#include "PrefixHeader.pch"
#endif

#include <memory>

#include "FBMemoryManager.h"