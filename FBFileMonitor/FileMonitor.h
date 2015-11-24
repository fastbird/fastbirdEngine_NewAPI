#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBCommonHeaders/Observable.h"
#include "IFileChangeObserver.h"
namespace fastbird{
	DECLARE_SMART_PTR(FileMonitor);
	class FB_DLL_FILEMONITOR FileMonitor : public Observable<IFileChangeObserver>{
		DECLARE_PIMPL_NON_COPYABLE(FileMonitor);
		FileMonitor();
		~FileMonitor();

	public:
		
		static FileMonitorPtr Create();
		static FileMonitor& GetInstance();
		bool HasInstance();

		void StartMonitor(const char* dirPath);
		void Check();

		void IgnoreMonitoringOnFile(const char* filepath);
		void ResumeMonitoringOnFile(const char* filepath);
	};
}