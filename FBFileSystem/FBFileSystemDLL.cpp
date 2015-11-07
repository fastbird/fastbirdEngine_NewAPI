#include "stdafx.h"
#include "../FBDebugLib/FBDebug.h"

using namespace fastbird;
class DLLInit{
public:
	DLLInit(){
		Debug::Init(_T("FileSystem.log"), Debug::BackUp, 5);
	}
	~DLLInit(){

	}
};

static DLLInit sDllInit;