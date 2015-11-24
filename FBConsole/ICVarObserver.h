#pragma once
namespace fastbird{
	struct CVar;
	class ICVarObserver
	{
	public:
		enum{
			Default
		};
		virtual bool OnChangeCVar(CVar* pCVar) = 0;
	};
}