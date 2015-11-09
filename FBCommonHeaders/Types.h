#pragma once
typedef unsigned char BYTE;
namespace fastbird{
	typedef double Real;
}
//#include <minwindef.h>

#define DECLARE_NON_COPYABLE(className) \
	className(const className&) = delete;\
	className& className::operator= (const className&) = delete;

#define DECLARE_PIMPL(className) \
	class className##Impl; \
	className##Impl* mImpl; \
	className(const className&) = delete;\
	className& className::operator= (const className&) = delete