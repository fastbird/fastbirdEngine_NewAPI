#if defined(_PLATFORM_WINDOWS)
    #define _FBFileSystemDLL __declspec(dllexport)
#else
    #include "PrefixHeader.pch"
#endif
#include <boost/filesystem.hpp>