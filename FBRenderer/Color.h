#pragma once
#include "FBCommonHeaders/Types.h"

namespace fastbird
{
	class Color
	{
	public:
		const static Color White;
		const static Color Black;
		const static Color Red;
		const static Color BrightRed;
		const static Color DarkGray;
		const static Color Gray;
		const static Color Silver;
		const static Color Green;
		const static Color Yellow;
		const static Color Blue;
		const static Color SkyBlue;
		const static Color Zero;

		struct RGBA
		{
			BYTE r;
			BYTE g;
			BYTE b;
			BYTE a;
		};
		static unsigned FixColorByteOrder(unsigned c);

		static Color RandomColor();

		Color();
		Color(const Vec3& color);
		Color(const Vec4& color);
		Color(Real r, Real g, Real b, Real a);
		Color(Real r, Real g, Real b);
		Color(LPCTSTR str);
		explicit Color(unsigned int c);

		// when you want to send the data to gpu
		unsigned int Get4Byte() const;
		// when you want to create hexa string
		unsigned int Get4ByteReversed() const;
		operator unsigned int() const;
		const Vec4& GetVec4() const;
		Color operator* (Real scalar) const;
		Color operator*(const Color& other) const;
		Color& operator*= (Real scalar);
		Color& operator*= (const Color& c);
		Color operator+ (const Color& r) const;
		bool operator== (const Color& other) const;
		void SetColor(Real r, Real g, Real b, Real a = 1.f);
		Real r() const;
		Real g() const;
		Real b() const;
		Real a() const;

		Real& r();
		Real& g();
		Real& b();
		Real& a();

	private:
		Vec4 mValue;
	};

	inline Color Random(const Color& min, const Color& max);
}