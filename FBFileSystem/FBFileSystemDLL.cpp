#include "stdafx.h"
#include "FBFileSystem.h"
#include "../FBDebugLib/FBDebug.h"
using namespace fastbird;

class DLLInit{
public:
	DLLInit(){
		auto filepath = _T("FileSystem.log");
		FileSystem::BackupFile(filepath, 5);
		Debug::Init(filepath);
	}
	~DLLInit(){
		Debug::Release();
	}
};

static DLLInit sDllInit;
