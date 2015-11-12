#include "stdafx.h"
#include "FileSystem.h"
#include "DirectoryIterator.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"
using namespace fastbird;

static bool gLogginStarted = false;
void FileSystem::StartLoggingIfNot(const char* path){
	if (gLogginStarted)
		return;
	auto filepath = "FileSystem.log";
	FileSystem::BackupFile(filepath, 5);
	Logger::Init(filepath);
	gLogginStarted = true;
}

void FileSystem::StopLogging(){
	if (!gLogginStarted)
		return;

	Logger::Release();
}

bool FileSystem::Exists(const char* path){
	return boost::filesystem::exists(path);
}

int FileSystem::Rename(const char* path, const char* newpath){
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
		Logger::Log(FormatString(FB_DEFAULT_LOG_ARG, err.what()));
	}
	
	return NO_ERROR;
}

bool FileSystem::Remove(const char* path){	
	bool ret = true;
	try{
		ret = boost::filesystem::remove(path);
	}
	catch (boost::filesystem::filesystem_error& err){
		Logger::Log(FormatString(FB_DEFAULT_LOG_ARG, err.what()));
	}	
	return ret;
}

TString FileSystem::ReplaceExtension(const char* path, const char* ext){
	boost::filesystem::path boostPath(path);
	boostPath.replace_extension(ext);
	return boostPath.generic_string();
}

const char* FileSystem::GetExtension(const char* path){
	size_t len = strlen(path);
	while (--len)
	{
		if (path[len] == '.')
		{
			return &path[len];
		}
	}
	return "";
}

void FileSystem::BackupFile(const char* filepath, unsigned numKeeping) {
	auto backupPath = FileSystem::ReplaceExtension(filepath, "");
	auto extension = FileSystem::GetExtension(filepath);
	for (int i = (int)numKeeping - 1; i > 0; --i){
		auto oldPath = FormatString("%s_bak%d.%s", backupPath.c_str(), i, extension);
		auto newPath = FormatString("%s_bak%d.%s", backupPath.c_str(), i + 1, extension);
		FileSystem::Rename(oldPath, newPath);
	}
	auto newPath = FormatString("%s_bak%d.%s", backupPath.c_str(), 1, extension);
	FileSystem::Rename(filepath, newPath);
}

DirectoryIteratorPtr FileSystem::GetDirectoryIterator(const char* filepath, bool recursive){
	return DirectoryIteratorPtr(new DirectoryIterator(filepath, recursive),
		[](DirectoryIterator* obj){delete obj; });
}