/**
\defgroup FBCommonHeaders
A group containing header files only.
*/
#pragma once
#define FBCommonHeaders_platform_h

#if defined(_WIN32)
	#define _PLATFORM_WINDOWS_
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
	#if TARGET_IPHONE_SIMULATOR
		#error "Platform is not supported"
	#elif TARGET_OS_IPHONE
		#error "Platform is not supported"
	#elif TARGET_OS_MAC
		#define _PLATFORM_MAC_
	#else
	#   error "Unknown Apple platform"
	#endif
#else
	#error "Platform is not supported"
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
	#if defined(FB_DLL_PROJECT)
		#ifdef __GNUC__
			#define FB_DLL_PUBLIC __attribute__((dllexport))
			#define FB_PLUGIN_DLL_PUBLIC __attribute__((dllimport))
		#else
			#define FB_DLL_PUBLIC __declspec(dllexport)
			#define FB_PLUGIN_DLL_PUBLIC __declspec(dllimport)
		#endif
	#elif defined(FB_PLUGIN_DLL_PROJECT)
		#ifdef __GNUC__
			#define FB_DLL_PUBLIC __attribute__((dllimport))
			#define FB_PLUGIN_DLL_PUBLIC __attribute__((dllexport))
		#else
			#define FB_DLL_PUBLIC __declspec(dllimport)
			#define FB_PLUGIN_DLL_PUBLIC __declspec(dllexport)
		#endif
	#else
		#ifdef __GNUC__
			#define FB_DLL_PUBLIC __attribute__((dllimport))
			#define FB_PLUGIN_DLL_PUBLIC __attribute__(dllimport)
		#else
			#define FB_DLL_PUBLIC __declspec(dllimport)
			#define FB_PLUGIN_DLL_PUBLIC __declspec(dllimport)
		#endif
	#endif
	#define FB_DLL_HIDDEN
#else
	#if __GNUC__ >=4
		#define FB_DLL_PUBLIC __attribute__((visibility("default")))
		#define FB_DLL_HIDDEN __attribute__ ((visibility("hidden")))
	#else
		#define FB_DLL_PUBLIC
		#define FB_DLL_HIDDEN
	#endif
#endif

#if !defined(FBCommonHeaders_Types_h)
#include "Types.h"
#endif