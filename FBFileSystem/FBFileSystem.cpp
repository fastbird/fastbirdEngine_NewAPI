#include "stdafx.h"
#include "FBFileSystem.h"
#include "../FBDebugLib/FBDebug.h"
#include "../FBStringLib/FBStringLib.h"
using namespace fastbird;

#if defined(UNICODE)
#define OutputException(err) Debug::Output(AnsiToWide(err.what()))
#else
#define OutputException(err) Debug::Output(err.what())
#endif


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