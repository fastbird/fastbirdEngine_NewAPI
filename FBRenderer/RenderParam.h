#pragma once
namespace fastbird{
	class RenderTarget;
	struct RenderParam{
		/** enum RENDER_PASS.
		*/
		int mRenderPass; 
		/** enum IRenderable::RenderableType or user defined custom value
		*/
		int mFilter; 
		int mReserved[2]; /// reserved.
		/** Render only special renderables.
		Special Renderables are like Cloud Volume, SkySphere or Full-screen effects.
		By specifying \a mFilter you can render only specific type of renderable.
		*/
		bool mSpecialRenderables;		
		bool mBoolReserved[3]; /// reserved.
		
	};

	/** For get a feedback from Renderables.
	*/
	struct RenderParamOut{
		int mReserved[4]; /// reserved.
		bool mSilouetteRendered;	/// set true, whenerver a Renderable touches silouette target.
		bool mBoolReserved[3]; /// reserved.
	};
}