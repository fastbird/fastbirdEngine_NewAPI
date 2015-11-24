#pragma once
namespace fastbird
{
	DECLARE_SMART_PTR(UIObject);
	class UIObject
	{
		DECLARE_PIMPL_NON_COPYABLE(UIObject);
		UIObject();
		~UIObject();

	public:
		static UIObjectPtr Create(const Vec2I& renderTargetSize);

		// debug data.
		IWinBase* mOwnerUI;
		std::string mTypeString;

		//-------------------------------------------------------------------------
		// IUIObject interfaces
		//-------------------------------------------------------------------------
		void SetTexCoord(Vec2 coord[], DWORD num, unsigned index=0);
		void ClearTexCoord(unsigned index = 0);
		void SetColors(DWORD colors[], DWORD num);

		void SetUIPos(const Vec2I& pos);
		const Vec2I& GetUIPos() const;

		void SetUISize(const Vec2I& size);
		const Vec2I& GetUISize() const;
		
		void SetAlpha(float alpha);

		void SetText(const wchar_t* s);
		void SetTextOffset(const Vec2I& offset);
		const Vec2I& GetTextOffset() const;
		void SetTextOffsetForCursorMovement(const Vec2I& offset);
		Vec2I GetTextStartWPos() const;

		void SetTextColor(const Color& c);
		void SetTextSize(float size);
		const RECT& GetRegion() const;
		void SetDebugString(const char* string);
		void SetNoDrawBackground(bool flag);		
		void SetUseScissor(bool use, const RECT& rect);
		void SetSpecialOrder(int order);
		int GetSpecialOrder() const;

		void SetMultiline(bool multiline);
		void SetDoNotDraw(bool doNotDraw);
		void SetRenderTargetSize(const Vec2I& rtSize);
		const Vec2I& GetRenderTargetSize() const;
		bool HasTexCoord() const;
		void EnableLinearSampler(bool linear);

		void PreRender();
		void Render();		

		void SetMaterial(const char* name);
		MaterialPtr GetMaterial() const;

		
		void UpdateRegion();
		static void ClearSharedRS();

		

	private:
		void PrepareVBs();
	};
}