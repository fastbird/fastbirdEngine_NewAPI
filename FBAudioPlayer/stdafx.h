#pragma comment(lib, "libogg.lib")
#pragma comment(lib, "libvorbis.lib")
#pragma comment(lib, "OpenAL32.lib")
#pragma warning (disable : 4251)

#include "FBCommonHeaders/platform.h"

#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <assert.h>
#include <vorbis/codec.h>

#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"

namespace fastbird{
	void Error(const char* szFmt, ...);
	void Log(const char* szFmt, ...);
}