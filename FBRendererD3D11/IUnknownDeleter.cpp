#include "stdafx.h"
#include "IUnknownDeleter.h"
using namespace fastbird;
void IUnknownDeleter::operator()(IUnknown* p){
	p->Release();
}