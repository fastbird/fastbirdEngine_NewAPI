#include "FBMemoryManager.h"
#include "../FBCommonHeaders/FBString.h"
#include "../FBDebugLib/FBDebug.h"
#include <mutex>
#include <map>
#include <fstream>
namespace fastbird
{
	std::recursive_mutex gMutex;
	unsigned long long gNumMemoryAllocation = 0;	
	struct MemLoc
	{
		MemLoc(LPCTSTR file, size_t line, LPCTSTR func)
		{
			if (file)  mFile = file;
			mLine = line;
			if (func) mFunc = func;
		}
		MemLoc()
		{}
		TString mFile;
		size_t mLine;
		TString mFunc;
	};
	typedef std::map<void*, MemLoc> LINEDATA;
	LINEDATA& GetMemAllocLines()
	{
		static LINEDATA gMemoryAllocLines;
		return gMemoryAllocLines;
	}

	std::_tofstream& GetMemStatFile()
	{
		static std::_tofstream gMemoryFile(_T("memory.txt"));
		return gMemoryFile;
	}

	//-----------------------------------------------------------------------
	void* AllocBytes(size_t size, LPCTSTR file, size_t line, LPCTSTR func)
	{
		void* p = malloc(size);
		if (!p)
			throw std::bad_alloc();

		std::lock_guard<std::recursive_mutex> lock(gMutex);		
		++gNumMemoryAllocation;		
		GetMemAllocLines()[p] = MemLoc(file, line, func);
		return p;
	}

	void* AllocBytesAligned(size_t size, size_t align, LPCTSTR file, size_t line, LPCTSTR func)
	{
		void* p = _aligned_malloc(size, align);
		if (!p)
			throw std::bad_alloc();

		std::lock_guard<std::recursive_mutex> lock(gMutex);
		++gNumMemoryAllocation;		
		GetMemAllocLines()[p] = MemLoc(file, line, func);
		return p;
	}

	//-----------------------------------------------------------------------
	void DeallocBytes(void* ptr, LPCTSTR file, size_t line, LPCTSTR func)
	{
		if (!ptr)
			return;
		std::lock_guard<std::recursive_mutex> lock(gMutex);
		auto it = GetMemAllocLines().find(ptr);
		if (it != GetMemAllocLines().end())
		{
			/*if (file &&  it->second.mFile != file)
			{
			if (strstr(file, "smartptr.h") == 0 && strstr(file, "SmartPtr.h") == 0)
			{
			Log("Memory(%s, %d, %s) is not deleted in the file where it was allocated. deallocated = (%s, %d)",
			it->second.mFile.c_str(), it->second.mLine, it->second.mFunc.c_str(), file, line);
			}
			}*/
			GetMemAllocLines().erase(it);
		}
		--gNumMemoryAllocation;
		free(ptr);
	}

	//-----------------------------------------------------------------------
	void DeallocBytesAligned(void* ptr, LPCTSTR file, size_t line, LPCTSTR func)
	{
		if (!ptr)
			return;
		std::lock_guard<std::recursive_mutex> lock(gMutex);

		auto it = GetMemAllocLines().find(ptr);
		if (it != GetMemAllocLines().end()){
			/*if (file && it->second.mFile != file)
			{
			Log("Memory(%s, %d, %s) is not deleted in the file where it was allocated. deallocated = (%s, %d)",
			it->second.mFile.c_str(), it->second.mLine, it->second.mFunc.c_str(), file, line);
			}*/
			GetMemAllocLines().erase(it);
			--gNumMemoryAllocation;
			_aligned_free(ptr);
		}

	}

	//-----------------------------------------------------------------------
	void FBReportMemoryForModule()
	{
		LINEDATA& m = GetMemAllocLines();
		auto it = m.begin();
		auto itEnd = m.end();
		for (; it != itEnd; ++it)
		{
			TCHAR buffer[500];
			_tsprintf(buffer, 500, _T("%s(%d) : memory(%x) not released \n"), it->second.mFile.c_str(), it->second.mLine, it->first);
			Debug::Output(buffer);
		}
	}
}