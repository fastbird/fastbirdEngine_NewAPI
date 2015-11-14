#pragma once
#include <vector>
namespace fastbird{
	struct ShaderDefine
	{
		ShaderDefine();
		ShaderDefine(const char* _name, const char* _value);
		bool operator==(const ShaderDefine& b) const;
		bool operator!=(const ShaderDefine& b) const;
		bool operator< (const ShaderDefine& b) const;
		std::string name;
		std::string value;
	};
	typedef std::vector<ShaderDefine> SHADER_DEFINES;
}