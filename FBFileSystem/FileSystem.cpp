#include "stdafx.h"
#include "FileSystem.h"
#include "DirectoryIterator.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"
using namespace fastbird;

#if defined(UNICODE)
#define OutputException(err) Logger::Output(AnsiToWide(err.what()))
#else
#define OutputException(err) Logger::Output(err.what())
#endif

static bool gLogginStarted = false;
void FileSystem::StartLoggingIfNot(LPCTSTR path){
	if (gLogginStarted)
		return;
	auto filepath = _T("FileSystem.log");
	FileSystem::BackupFile(filepath, 5);
	Logger::Init(filepath);
	gLogginStarted = true;
}

void FileSystem::StopLogging(){
	if (!gLogginStarted)
		return;

	Logger::Release();
}

bool FileSystem::Exists(LPCTSTR path){
	return boost::filesystem::exists(path);
}

int FileSystem::Rename(LPCTSTR path, LPCTSTR newpath){
	if (!Exists(path)){
		return RENAME_NO_SOURCE;
	}

	if (Exists(newpath)){
		return RENAME_DEST_EXISTS;
	}
	
	try{
		boost::filesystem::rename(path, newpath);
	}
	catch (boost::filesystem::filesystem_error& err){
		OutputException(err);		
	}
	
	return NO_ERROR;
}

bool FileSystem::Remove(LPCTSTR path){	
	bool ret = true;
	try{
		ret = boost::filesystem::remove(path);
	}
	catch (boost::filesystem::filesystem_error& err){
		OutputException(err);
	}	
	return ret;
}

TString FileSystem::ReplaceExtension(LPCTSTR path, LPCTSTR ext){
	boost::filesystem::path boostPath(path);
	boostPath.replace_extension(ext);
	return boostPath._tgeneric_string();
}

LPCTSTR FileSystem::GetExtension(LPCTSTR path){
	size_t len = _tstrlen(path);
	while (--len)
	{
		if (path[len] == _T('.'))
		{
			return &path[len];
		}
	}
	return _T("");
}

void FileSystem::BackupFile(LPCTSTR filepath, unsigned numKeeping) {
	auto backupPath = FileSystem::ReplaceExtension(filepath, _T(""));
	auto extension = FileSystem::GetExtension(filepath);
	for (int i = (int)numKeeping - 1; i > 0; --i){
		auto oldPath = FormatString(_T("%s_bak%d.%s"), backupPath.c_str(), i, extension);
		auto newPath = FormatString(_T("%s_bak%d.%s"), backupPath.c_str(), i + 1, extension);
		FileSystem::Rename(oldPath, newPath);
	}
	auto newPath = FormatString(_T("%s_bak%d.%s"), backupPath.c_str(), 1, extension);
	FileSystem::Rename(filepath, newPath);
}

DirectoryIteratorPtr FileSystem::GetDirectoryIterator(LPCTSTR filepath, bool recursive){
	return DirectoryIteratorPtr(new DirectoryIterator(filepath, recursive),
		[](DirectoryIterator* obj){delete obj; });
}