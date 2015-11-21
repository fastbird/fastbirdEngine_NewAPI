#pragma warning (disable : 4251 4275 4819)

#if defined(_WIN32) 
#define FB_DLL_COLLADA __declspec(dllexport)
#define FB_DLL_ANIMATION __declspec(dllimport)
#define FB_DLL_FILESYSTEM __declspec(dllimport)
#else
#define FB_DLL_ANIMATION
#endif

// Open Colladas
#pragma comment(lib, "OpenCOLLADASaxFrameworkLoader.lib")
#pragma comment(lib, "OpenCOLLADAFramework.lib")
#pragma comment(lib, "OpenCOLLADABaseUtils.lib")
#pragma comment(lib, "GeneratedSaxParser.lib")
#pragma comment(lib, "pcre.lib")
#pragma comment(lib, "xml.lib")
#pragma comment(lib, "UTF.lib")
#pragma comment(lib, "MathMLSolver.lib")

#include <COLLADASaxFWLLoader.h>
#include <COLLADAFW.h>