#pragma once
#include <vector>
#include <stack>
#include <map>
#include "RendererStructs.h"
#include "FBMathLib/Vec2I.h"
#include  "TextTags.h"
#include "../EssentialEngineData/shaders/Constants.h"

namespace fastbird{
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(VertexBuffer);
	DECLARE_SMART_PTR(Shader);
	DECLARE_SMART_PTR(InputLayout);
	DECLARE_SMART_PTR(Material);
	class SCharDescr;
	typedef DEFAULT_INPUTS::V_PCTB  FontVertex;
	DECLARE_SMART_PTR(Font);
	class Font
	{
		DECLARE_PIMPL_NON_COPYABLE(Font);
		Font();

	public:
		enum EFontTextEncoding
		{
			NONE,
			UTF8,
			UTF16
		};

		enum FONT_ALIGN
		{
			FONT_ALIGN_LEFT = 0,
			FONT_ALIGN_CENTER,
			FONT_ALIGN_RIGHT,
			FONT_ALIGN_JUSTIFY
		};

		static FontPtr Create();		
		//--------------------------------------------------------------------
		// IFont
		//--------------------------------------------------------------------
		virtual int Init(const char *fontFile);
		virtual void SetTextEncoding(EFontTextEncoding encoding);

		virtual void PreRender(){}
		virtual void Render(){}
		virtual void PostRender(){}
		virtual void Write(float x, float y, float z, unsigned int color,
			const char *text, int count, FONT_ALIGN mode);

		virtual void SetHeight(float h);
		virtual float GetHeight() const;
		virtual float GetBaseHeight() const;
		virtual void SetBackToOrigHeight();
		virtual float GetTextWidth(const char *text, int count = -1, float *minY = 0, float *maxY = 0);
		virtual std::wstring InsertLineFeed(const char *text, int count, unsigned wrapAt, float* outWidth, unsigned* outLines);
		virtual void PrepareRenderResources();
		virtual void SetRenderStates(bool depthEnable = false, bool scissorEnable = false);

		float GetBottomOffset();
		float GetTopOffset();

		virtual void SetRenderTargetSize(const Vec2I& rtSize);
		virtual void RestoreRenderTargetSize();
		std::wstring StripTags(const wchar_t* text);

	protected:
		friend class FontLoader;

		static const unsigned int MAX_BATCH;

		bool ApplyTag(const char* text, int start, int end, int& x, int& y);
		TextTags::Enum GetTagType(const char* tagStart, int length, char* buf = 0) const;
		void InternalWrite(int x, int y, float z, const char *text, int count, int spacing = 0);
		void Flush(int page, const FontVertex* pVertices, unsigned int vertexCount);

		int AdjustForKerningPairs(int first, int second);
		SCharDescr *GetChar(int id);

		int GetTextLength(const char *text);
		int SkipTags(const char* text, TextTags::Enum* tag = 0, int* imgLen = 0);
		int GetTextChar(const char *text, int pos, int *nextPos = 0);
		int FindTextChar(const char *text, int start, int length, int ch);

		
	};
}