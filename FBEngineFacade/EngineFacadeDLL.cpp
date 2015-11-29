#include "stdafx.h"
#include "FBFileSystem/FileSystem.h"
using namespace fastbird;

class DLLInit{
public:
	DLLInit(){
		auto filepath = "_FBEngineFacade.log";
		FileSystem::BackupFile(filepath, 5, "Backup_Log");
		Logger::Init(filepath);
	}
	~DLLInit(){
		Logger::Release();
	}
};

static DLLInit sDllInit;