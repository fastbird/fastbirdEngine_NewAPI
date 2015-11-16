// Should not define data in this file.
// Only typedefs or defines are allowed.
#pragma once
#define FBCommonHeaders_Types_h
#include <utility>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

#define FB_DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name


namespace fastbird{
	typedef double Real;
	typedef unsigned char BYTE;
	typedef unsigned char UINT8;
	typedef unsigned short USHORT;
	typedef unsigned int UINT;	
	typedef unsigned long DWORD;
	typedef unsigned __int64 UINT64;
	typedef __int64 INT64;
	typedef __int64 HWindowId;

	typedef std::vector<std::string> StringVector;
	typedef std::vector<std::wstring> WStringVector;
	
	// unsigned int : safe for 828 'days' at 60 frames/sec
	// unsigned long long : safe for 9749040289 'years' at 60 frames/sec
	typedef unsigned int FRAME_PRECISION;
	typedef Real TIME_PRECISION;

	FB_DECLARE_HANDLE(HWindow);
	typedef std::lock_guard<std::mutex> MutexLock;

	typedef std::tuple<int, int> Vec2ITuple;
	typedef std::tuple<int, int, int> Vec3ITuple;
	typedef std::tuple<Real, Real> Vec2Tuple;
	typedef std::tuple<Real, Real, Real> Vec3Tuple;
	typedef std::tuple<Real, Real, Real, Real> Vec4Tuple;
	typedef struct QUAT_TUPLE{
		QUAT_TUPLE(){};
		QUAT_TUPLE(Real w, Real x, Real y, Real z)
			:value(w, x, y, z)	{}
		std::tuple < Real, Real, Real, Real > value;
	} QuatTuple;
	typedef std::tuple <
		// rotation
		Real, Real, Real,
		Real, Real, Real,
		Real, Real, Real,
		// quaternion
		Real, Real, Real, Real,
		// translation
		Real, Real, Real,
		// scale
		Real, Real, Real,
		// itentity, RSSeperated, UniformScale
		bool, bool, bool> TransformationTuple;
}

#define DECLARE_NON_COPYABLE(className) \
	className(const className&) = delete;\
	className& operator= (const className&) = delete

#define DECLARE_PIMPL(className) \
	class Impl; \
	std::shared_ptr<Impl> mImpl

#define DECLARE_PIMPL_NON_COPYABLE(className) \
	DECLARE_PIMPL(className); \
	DECLARE_NON_COPYABLE(className)

#define DECLARE_PIMPL_CLONEABLE(className) \
	DECLARE_PIMPL(className); \
protected:\
	className(const className&);\
	className& operator= (const className&) = delete

#define DECLARE_SMART_PTR(className) \
	class className;\
	typedef std::shared_ptr<className> className##Ptr;\
	typedef std::shared_ptr<const className> className##ConstPtr;\
	typedef std::weak_ptr<className> className##WeakPtr

#define DECLARE_SMART_PTR_STRUCT(className) \
	struct className;\
	typedef std::shared_ptr<className> className##Ptr;\
	typedef std::weak_ptr<className> className##WeakPtr

#define IMPLEMENT_STATIC_CREATE(className)\
	className##Ptr className##::Create(){\
		return className##Ptr(new className, [](className* obj){delete obj;});\
	}