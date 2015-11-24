#pragma once

namespace fastbird
{
	//--------------------------------------------------------------------------------
	class IFileChangeObserver
	{
	public:
		enum{
			FileChange_Engine,			
			FileChange_Game,
		};
		virtual bool OnFileChanged(const char* file) = 0;
	};
}