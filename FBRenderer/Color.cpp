#include "stdafx.h"
#include "Color.h"
#include "FBStringLib/StringLib.h"
#include "FBStringMathLib/StringMathConverter.h"
#include "FBStringLib/StringConverter.h"

namespace fastbird
{
const Color Color::White(1, 1, 1);
const Color Color::Black(0, 0, 0);
const Color Color::Red(1, 0, 0);
const Color Color::BrightRed(1, 0.2, 0.2);
const Color Color::DarkGray(0.15f, 0.15f, 0.15f);
const Color Color::Gray(0.5f, 0.5f, 0.5f);
const Color Color::Silver(0.8f, 0.8f, 0.8f);
const Color Color::Green(0, 1, 0);
const Color Color::Yellow(1, 1, 0);
const Color Color::Blue(0, 0, 1);
const Color Color::SkyBlue(0, 0.5f, 1);
const Color Color::Zero(0, 0, 0, 0);

unsigned Color::FixColorByteOrder(unsigned c)
{
	RGBA color;
	color.r = c >> 24;
	color.g = c >> 16 & 0xff;
	color.b = c >> 8 & 0xff;
	color.a = c & 0xff;
	return *(unsigned*)&color;
}

Color::Color(LPCTSTR str)
{
	if (str[0] == _T('0') && str[1] == _T('x'))
	{	
		if (_tstrlen(str) == 8)
		{
			TString strColor = str;
			strColor += _T("ff");
			*this = Color(Color::FixColorByteOrder(StringConverter::ParseHexa(strColor.c_str())));
		}
		else
		{
			*this = Color(Color::FixColorByteOrder(StringConverter::ParseHexa(str)));
		}
	}
	else
	{
		mValue =  StringMathConverter::ParseVec4(str, Vec4::ZERO);
	}
}

Color Color::RandomColor(){
	return Color(Random(), Random(), Random(), 1.f);
}

}