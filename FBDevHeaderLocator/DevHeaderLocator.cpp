// FBDevHeaderLocator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define FB_DLL_PUBLIC __declspec(dllimport)
#include "FBFileSystem/FileSystem.h"
#include "FBFileSystem/DirectoryIterator.h"
#include <iostream>

using namespace fastbird;
void LocateHeaders(LPCTSTR dest){
	auto it = FileSystem::GetDirectoryIterator(_T("."), true);
	bool isDir = false;
	auto filename = it->GetNextFileName(isDir);
	while (_tstrlen(filename) > 0){
		std::_tcout << filename << _T(" IsDir: ") << std::boolalpha << isDir << std::noboolalpha << std::endl;
		filename = it->GetNextFileName(isDir);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	FileSystem::StartLoggingIfNot(_T("FileSystem.log"));
	LocateHeaders(_T("include/"));
	FileSystem::StopLogging();
	return 0;
}

