#pragma once
#include <utility>
namespace fastbird{
	typedef double Real;
	typedef unsigned char BYTE;
	typedef unsigned char UINT8;
	typedef unsigned int UINT;
	typedef unsigned long DWORD;
	typedef unsigned __int64 UINT64;
	typedef __int64 INT64;
	typedef __int64 HWindowId;
	typedef __int64 HWindow;

	typedef std::pair<int, int> CoordinatesI;
	typedef std::pair<Real, Real> CoordinatesR;
}

#define DECLARE_NON_COPYABLE(className) \
	className(const className&) = delete;\
	className& className::operator= (const className&) = delete;

#define DECLARE_PIMPL(className) \
	class className##Impl; \
	className##Impl* mImpl; \
	DECLARE_NON_COPYABLE(className)

#define DECLARE_SMART_PTR(className) \
	class className;\
	typedef std::shared_ptr<className> className##Ptr;\
	typedef std::weak_ptr<className> className##WeakPtr