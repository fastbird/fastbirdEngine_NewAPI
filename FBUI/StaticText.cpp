#include "StdAfx.h"
#include "StaticText.h"
#include "KeyboardCursor.h"

#include "IUIManager.h"

namespace fastbird
{

StaticText::StaticText()
	: WinBase()
	, mCursorPos(0)
	, mPasswd(false)
{
	mUIObject = UIObject::Create(GetRenderTargetSize());
	mUIObject->mOwnerUI = this;
	mUIObject->mTypeString = ComponentType::ConvertToString(GetType());
	mUIObject->SetTextColor(mTextColor);
	SetProperty(UIProperty::BACK_COLOR, gFBUIManager->GetStyleString(Styles::StaticTextBack));
}

StaticText::~StaticText()
{
}

void StaticText::GatherVisit(std::vector<IUIObject*>& v)
{
	if (!mVisibility.IsVisible())
		return;
	v.push_back(mUIObject);	
	__super::GatherVisit(v);
}

}