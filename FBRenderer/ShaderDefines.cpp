#include "stdafx.h"
#include "ShaderDefines.h"
namespace fastbird{
	ShaderDefine::ShaderDefine(){
	}

	ShaderDefine::ShaderDefine(const char* _name, const char* _value)
		: name(_name), value(_value)
	{

	}

	bool ShaderDefine::operator == (const ShaderDefine& b) const{
		if (name == b.name && value == b.value)
			return true;

		return false;
	}

	bool ShaderDefine::operator!=(const ShaderDefine& b) const{
		return !operator==(b);
	}

	bool ShaderDefine::operator< (const ShaderDefine& b) const{
		if (name < b.name)
			return true;
		else if (name == b.name){
			return value < b.value;
		}
	}
}