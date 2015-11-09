#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
    #define FBFileSystemDLL __declspec(dllexport)
#else
    #include "PrefixHeader.pch"
#endif
#include <boost/filesystem.hpp>