#pragma once
#include "RendererEnums.h"
#include "RendererStructs.h"

namespace fastbird{	

	typedef intptr_t HWND_ID;
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(RenderTarget);
	DECLARE_SMART_PTR(Renderer);
	/** Render vertices with a specified material	
	Rednerer handles vertex/index data, materials, textures, shaders,
	render states, lights and render targets.
	\ingroup FBRenderer
	*/
	class FBRendererDLL Renderer{	
		RendererWeakPtr mMe;

		DECLARE_PIMPL(Renderer);
		Renderer();		

	public:
		/** Create an empty Renderer. You have the owner ship of the pointer.
		*/
		static RendererPtr CreateRenderer();

		/** Create a Renderer with the render engine you specified in \a renderEngineName 
		\param rendererName The renderer plug-in name. You can specify "FBRendererD3D11" for
		the default renderer on Windows, and "FBRendererGL41" for Mac. \n
		You have the owner ship of the pointer.
		*/
		static RendererPtr CreateRenderer(const char* renderEngineName);

		static RendererPtr GetRenderer();
	public:
		~Renderer();

		/** Prepare the render engine
		You don't call this function if you initiate the Renderer with the function
		 \code 
		 static RendererPtr CreateRenderer(const char* renderEngineName); 
		 \endcode
		 because the Renderer you got is already initialized the render engine.		 
		*/
		void PrepareRenderEngine(const char* renderEngineName);

		//-------------------------------------------------------------------
		// Resource Creation
		//-------------------------------------------------------------------
		RenderTargetPtr CreateRenderTarget();
		/** Load texture file asynchronously
		*/
		TexturePtr CreateTexture(const char* file, bool async);
		TexturePtr CreateTexture(const char* file, Texture* reloading, bool async);

		//-------------------------------------------------------------------
		// Resource Management
		//-------------------------------------------------------------------
		/** Reload texture in the \a textuerPath
		*/
		void ReloadTexture(const char* texturePath);
		
		//-------------------------------------------------------------------
		void SetCurrentRenderTarget(RenderTarget* renderTarget);
	};

}