#include "FBDebug.h"
#include "../FBCommonHeaders/platform.h"
#define _FBFileSystemDECL
#include "../FBFileSystem/FBFileSystem.h"
#include "../FBStringLib/FBStringLib.h"

#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif

#include <iostream>

using namespace fastbird;

static int sInitialized = false;
static std::ofstream sLogFile;

static void BackupFiles(LPCTSTR filepath, unsigned numKeeping){
	auto backupPath = FileSystem::ReplaceExtension(filepath, _T(""));
	auto extension = FileSystem::GetExtension(filepath);
	for (int i = (int)numKeeping - 1; i > 0; -- i){		
		auto oldPath = FormatString(_T("%s%d.%s_bak"), backupPath.c_str(), i, extension);
		auto newPath = FormatString(_T("%s%d.%s_bak"), backupPath.c_str(), i+1, extension);		
		FileSystem::Rename(oldPath, newPath);		
	}
	auto newPath = FormatString(_T("%s%d.%s_bak"), backupPath.c_str(), 1, extension);
	FileSystem::Rename(filepath, newPath);
}

void Debug::Init(LPCTSTR filepath, FileHandling handling, unsigned numKeeping){
	if (FileSystem::Exists(filepath)){
		if (handling == FileHandling::BackUp){
			BackupFiles(filepath, numKeeping);
		}		
	}
	sLogFile.open(filepath, std::ofstream::out);
	sInitialized = true;
}

void Debug::Release(){
	sInitialized = false;
	sLogFile.close();	
}

void Debug::Log(LPCTSTR str){
	if (sLogFile.is_open()){
		sLogFile << str;
		sLogFile.flush();
	}
	else{	
		// fallback
		Output(str);
	}
}
void Debug::Log(std::ofstream& file, LPCTSTR str){
	if (file.is_open()){
		file << str;
	}
	else{
		// fallback
		Output(str);
	}
}

void Debug::Output(LPCTSTR str){
#if defined(_PLATFORM_WINDOWS_)
	OutputDebugString(str);	
#else
	std::cerr << str;
#endif
}
