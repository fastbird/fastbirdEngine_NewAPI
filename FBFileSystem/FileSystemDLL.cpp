#include "stdafx.h"
#include "FileSystem.h"
#include "FBDebugLib/Logger.h"
using namespace fastbird;

class DLLInit{
public:
	DLLInit(){
		/*auto filepath = _T("FileSystem.log");
		FileSystem::BackupFile(filepath, 5);
		Logger::Init(filepath);*/
	}
	~DLLInit(){
		/*Logger::Release();*/
	}
};

static DLLInit sDllInit;
