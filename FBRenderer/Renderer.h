#pragma once

namespace fastbird{
	class Renderer;
	typedef std::shared_ptr<Renderer> RendererPtr;
	typedef std::weak_ptr<Renderer> RendererWeakPtr;
	
	/** Render vertices with a specified material
	Rednerer handles vertex/index data, materials, textures, shaders,
	render states, lights and render targets.
	*/
	class FBRendererDLL Renderer{	
		class RendererImpl;
		RendererImpl* mImpl;
		Renderer(const Renderer&) = delete;
		Renderer& Renderer::operator= (const Renderer&) = delete;

	public:
		Renderer();
		~Renderer();
		/** Create Renderer with the specified type
		\param rendererType "D3D11", "OpenGL4" are internally supported
		*/
		static Renderer* CreateRenderer(const char* rendererType);
	};

}