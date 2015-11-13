#pragma once
#include "Color.h"
namespace fastbird{

	DECLARE_SMART_PTR(ColorRamp);
	class ColorRamp
	{
		DECLARE_PIMPL(ColorRamp);
		ColorRamp();
		static void* operator new (size_t);
		static void* operator new[](size_t);

	public:
		ColorRampPtr Create();
		bool operator==(const ColorRamp& other) const;
		const Color& operator[] (int idx) const;

		struct Bar
		{
			float position;
			Color color;

			Bar(float pos, const Color& color_)
				:position(pos), color(color_){}
			bool operator < (const Bar& other) const {
				return position < other.position;
			}
			bool operator== (const Bar& other) const {
				return (position == other.position && color == other.color);
			}
		};

		void InsertBar(float pos, const Color& color);
		unsigned NumBars() const;
		Bar& GetBar(int idx);
		void GenerateColorRampTextureData(int textureWidth);
		void UpperAlign(float gap = 0.01f);
		void LowerAlign(float gap = 0.01f);
	};
}