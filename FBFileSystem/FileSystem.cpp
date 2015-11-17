/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "FileSystem.h"
#include "DirectoryIterator.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"
using namespace fastbird;

static bool gLogginStarted = false;
static boost::filesystem::path gWorkingPath;
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
		Logger::Log(FB_ERROR_LOG_ARG, err.what());
	}
	
	return NO_ERROR;
}

bool FileSystem::Remove(const char* path){	
	bool ret = true;
	try{
		ret = boost::filesystem::remove(path);
	}
	catch (boost::filesystem::filesystem_error& err){
		Logger::Log(FB_ERROR_LOG_ARG, err.what());
	}	
	return ret;
}

std::string FileSystem::ReplaceExtension(const char* path, const char* ext){
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

std::string FileSystem::GetName(const char* path){
	boost::filesystem::path filepath(path);
	return filepath.stem().generic_string();
}

std::string FileSystem::GetParentPath(const char* path){
	boost::filesystem::path filepath(path);
	return filepath.parent_path().generic_string();
}

std::string FileSystem::ConcatPath(const char* path1, const char* path2){
	return boost::filesystem::path(path1).concat(path2).generic_string();
}

void FileSystem::BackupFile(const char* filepath, unsigned numKeeping) {
	auto backupPath = FileSystem::ReplaceExtension(filepath, "");
	auto extension = FileSystem::GetExtension(filepath);
	for (int i = (int)numKeeping - 1; i > 0; --i){
		auto oldPath = FormatString("%s_bak%d.%s", backupPath.c_str(), i, extension);
		auto newPath = FormatString("%s_bak%d.%s", backupPath.c_str(), i + 1, extension);
		FileSystem::Rename(oldPath.c_str(), newPath.c_str());
	}
	auto newPath = FormatString("%s_bak%d.%s", backupPath.c_str(), 1, extension);
	FileSystem::Rename(filepath, newPath.c_str());
}

int FileSystem::CompareFileModifiedTime(const char* file1, const char* file2){
	try{
		auto time1 = boost::filesystem::last_write_time(file1);
		auto time2 = boost::filesystem::last_write_time(file2);
		if (time1 < time2)
			return -1;
		else if (time1 == time2)
			return 0;
		else
			return 1;

	}
	catch (boost::filesystem::filesystem_error& err){
		Logger::Log(FB_ERROR_LOG_ARG, err.what());
	}
	return 0;
}

bool FileSystem::SecurityOK(const char* filepath){
	if (gWorkingPath.empty()){
		gWorkingPath = boost::filesystem::current_path(); // absolute
	}
	auto abspath = boost::filesystem::absolute(filepath);
	if (abspath.generic_string().find(gWorkingPath.generic_string()) != std::string::npos)
		return true;

	return false;
}

BinaryData FileSystem::ReadBinaryFile(const char* path, std::streamoff& outLength){
	std::ifstream is(path, std::ios_base::binary);
	if (is)
	{
		is.seekg(0, is.end);
		outLength = is.tellg();
		is.seekg(0, is.beg);

		std::shared_ptr<char> buffer = std::shared_ptr<char>(new char[(unsigned int)outLength], [](char* obj){ delete[] obj; });
		is.read(buffer.get(), outLength);
		if (!is)
		{
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Only %u could be read for the file (%s)", is.gcount(), path).c_str());
		}
		is.close();
		return buffer;
	}
	else
		return 0;
}

void FileSystem::WriteBinaryFile(const char* path, char* data, size_t length){
	if (!data || length == 0 || path == 0)
		return;

	if (!SecurityOK(path))
	{
		Logger::Log(FB_ERROR_LOG_ARG, FormatString("FileSystem: SaveBinaryFile to %s has security violation.", path).c_str());
		return;
	}

	std::ofstream ofs(path, std::ios_base::binary | std::ios_base::trunc);
	if (!ofs)
	{
		Logger::Log(FB_ERROR_LOG_ARG, FormatString("FileSystem: Cannot open a file(%s) for writing.", path).c_str());
		return;
	}
	ofs.write(data, length);
}

//---------------------------------------------------------------------------
// Directory Operataions
//---------------------------------------------------------------------------
DirectoryIteratorPtr FileSystem::GetDirectoryIterator(const char* filepath, bool recursive){
	return DirectoryIteratorPtr(new DirectoryIterator(filepath, recursive),
		[](DirectoryIterator* obj){delete obj; });
}

bool FileSystem::CreateDirectory(const char* filepath){
	bool ret = true;
	try{
		ret = boost::filesystem::create_directories(filepath);
	}
	catch (boost::filesystem::filesystem_error& err){
		Logger::Log(FB_ERROR_LOG_ARG, err.what());
	}
	return ret;
}


//---------------------------------------------------------------------------
// System Folders
//---------------------------------------------------------------------------
std::string FileSystem::GetAppDataFolder(){
#if defined(_PLATFORM_WINDOWS_)
	PWSTR* path=0;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, path))){
		auto ret = std::string(WideToAnsi(*path));
		CoTaskMemFree(path);
		return ret;
	}	
#else
	assert(0 && "Not implemented");
#endif
	return std::string("./temp/");
}
