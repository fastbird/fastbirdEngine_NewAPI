#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(EngineFacade);
	class FB_DLL_ENGINEFACADE EngineFacade{
		DECLARE_PIMPL_NON_COPYABLE(EngineFacade);
		EngineFacade();
		~EngineFacade();

	public:
		static EngineFacadePtr Create();

		static const HWindowId INVALID_HWND_ID = (HWindowId)-1;
		
		/// \param pluginName "FBRendererD3D11" is provided.
		bool InitRenderer(const char* pluginName);
		bool InitCanvas(HWindow window, int width, int height);
		ScenePtr GetMainScene() const;
		void Update(TIME_PRECISION dt);
		void Render();
	};
}