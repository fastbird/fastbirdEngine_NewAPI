#include "stdafx.h"
#include "InputLayout.h"
using namespace fastbird;

class InputLayout::Impl
{
public:
	static Impl* sLastBinded;
	IPlatformInputLayoutPtr mPlatformInputLayout;

	void Bind(){
		if (sLastBinded == this)
			return;
		mPlatformInputLayout->Bind();
	}
	void SetPlatformInputLayout(IPlatformInputLayoutPtr layout){
		mPlatformInputLayout = layout;
	}

};

//---------------------------------------------------------------------------
IMPLEMENT_STATIC_CREATE(InputLayout);
InputLayout::InputLayout()
	:mImpl(new Impl)
{
}

void InputLayout::Bind(){
	mImpl->Bind();
}

void InputLayout::SetPlatformInputLayout(IPlatformInputLayoutPtr layout){
	mImpl->SetPlatformInputLayout(layout);
}