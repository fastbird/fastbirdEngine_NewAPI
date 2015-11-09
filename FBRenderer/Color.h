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

		Color(){}
		Color(const Vec3& color)
			: mValue(color, 1.0f)
		{
		}
		Color(const Vec4& color)
			: mValue(color)
		{
		}
		Color(Real r, Real g, Real b, Real a)
			: mValue(r, g, b, a)
		{
		}

		Color(Real r, Real g, Real b)
			: mValue(r, g, b, 1.f)
		{
		}

		Color(LPCTSTR str);

		explicit Color(unsigned int c)
		{
			RGBA* rgba = (RGBA*)&c;
			mValue.x = rgba->r / 255.0f;
			mValue.y = rgba->g / 255.0f;
			mValue.z = rgba->b / 255.0f;
			mValue.w = rgba->a / 255.0f;
		}

		// when you want to send the data to gpu
		unsigned int Get4Byte() const
		{
			RGBA color;
			color.r = BYTE(mValue.x * 255.f);
			color.g = BYTE(mValue.y * 255.f);
			color.b = BYTE(mValue.z * 255.f);
			color.a = BYTE(mValue.w * 255.f);
			return *(unsigned int*)&color;
		}

		// when you want to create hexa string
		unsigned int Get4ByteReversed() const
		{
			RGBA color;
			color.r = BYTE(mValue.w * 255.f);
			color.g = BYTE(mValue.z * 255.f);
			color.b = BYTE(mValue.y * 255.f); 
			color.a = BYTE(mValue.x * 255.f); 
			return *(unsigned int*)&color;
		}

		inline operator unsigned int() const { return Get4Byte(); }

		const Vec4& GetVec4() const { return mValue; }

		inline Color operator* (Real scalar) const
		{
			return Color(mValue*scalar);
		}

		inline Color operator*(const Color& other) const
		{
			return Color(mValue*other.mValue);
		}

		inline Color& operator*= (Real scalar)
		{
			mValue *= scalar;
			return *this;
		}

		inline Color& operator*= (const Color& c)
		{
			mValue *= c.mValue;
			return *this;
		}

		inline Color operator+ (const Color& r) const
		{
			return mValue + r.GetVec4();
		}

		bool operator== (const Color& other) const
		{
			return mValue==other.mValue;
		}

		void SetColor(Real r, Real g, Real b, Real a=1.f)
		{
			mValue.x = r;
			mValue.y = g;
			mValue.z = b;
			mValue.w = a;
		}

		Real r() const {return mValue.x;}
		Real g() const {return mValue.y;}
		Real b() const {return mValue.z;}
		Real a() const {return mValue.w;}

		Real& r(){ return mValue.x; }
		Real& g(){ return mValue.y; }
		Real& b(){ return mValue.z; }
		Real& a(){ return mValue.w; }

	private:
		Vec4 mValue;
	};

	inline Color Random(const Color& min, const Color& max)
	{
		return Color(Random(min.r(), max.r()), Random(min.g(), max.g()),
			Random(min.b(), max.b()));
	}
}