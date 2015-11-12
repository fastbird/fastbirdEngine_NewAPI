/**
\file FBMemoryManager.h
Provide a ceteralized method for memory allocation and deallocation
\author Jungwan Byun
\defgroup FBMemoryManagerLib
Provide a ceteralized method for memory allocation and deallocation
*/
#pragma once
#include <cstddef>
#include "FBCommonHeaders/String.h"
#ifdef NOT_USING_FB_MEMORY_MANAGER
#define FB_NEW(T) new T
#define FB_ARRAY_NEW(T, count) new T[count]
#define FB_DELETE(ptr) delete ptr
#define FB_ARRAY_DELETE(ptr) delete[] ptr
#else

namespace fastbird
{
	/// \ingroup FBMemoryManager
	void FBReportMemoryForModule();
    void* AllocBytes(size_t size, LPCTSTR file, size_t line, LPCTSTR func);
	void* AllocBytesAligned(size_t size, size_t align, LPCTSTR file, size_t line, LPCTSTR func);
	void DeallocBytes(void* prt, LPCTSTR file, size_t line, LPCTSTR func);
	void DeallocBytesAligned(void* ptr, LPCTSTR file, size_t line, LPCTSTR func);
	
	template <typename T>
	inline T* ConstructN(T* startp, size_t num)
	{
        for (size_t i = 0; i < num; ++i)
		{
			new (startp + i) T();
		}
		return startp;
	}

	template <typename T>
	inline void DestructN(T* startp)
	{
        size_t num = _msize(startp) / sizeof(T);
        for (size_t i = 0; i < num; ++i)
		{
			T* instance = (startp + i);
			instance->~T();
		}
	}

	template <typename T>
	inline void Delete(T* p, LPCTSTR file, size_t line, LPCTSTR func)
	{
		if (!p)
			return;
		p->~T();
		DeallocBytes(p, file, line, func);
	}

	template <typename T>
	inline void DeleteAligned(T* p, LPCTSTR file, size_t line, LPCTSTR func)
	{
		if (!p)
			return;
		p->~T();
		DeallocBytesAligned(p, file, line, func);
	}

	template <typename T>
	inline void DeleteArr(T* p, LPCTSTR file, size_t line, LPCTSTR func)
	{
		if (!p)
			return;
		fastbird::DestructN(p);
		DeallocBytes(p, file, line, func);
	}
}
/// \addtogroup FBMemoryManagerLib
/// @{
#define FB_NEW(T) new (fastbird::AllocBytes(sizeof(T), __TFILE__, __LINE__, __TFUNCTION__)) T
#define FB_ARRAY_NEW(T, count) fastbird::ConstructN(static_cast<T*>(fastbird::AllocBytes(sizeof(T)*count, __TFILE__, __LINE__, __TFUNCTION__)), count)
#define FB_DELETE(ptr) fastbird::Delete( (ptr), __TFILE__, __LINE__, __TFUNCTION__)
#define FB_ARRAY_DELETE(ptr) fastbird::DeleteArr(ptr, __TFILE__, __LINE__, __TFUNCTION__)
#define FB_SAFE_DELETE(ptr) (ptr) ? FB_DELETE((ptr)) : 0; (ptr) = 0;
#define FB_NEW_ALIGNED(T, A) new (fastbird::AllocBytesAligned(sizeof(T), A, __TFILE__, __LINE__, __TFUNCTION__)) T
#define FB_DELETE_ALIGNED(ptr) (ptr) ? fastbird::DeleteAligned( (ptr), __TFILE__, __LINE__, __TFUNCTION__) : 0;
/// @}
#endif