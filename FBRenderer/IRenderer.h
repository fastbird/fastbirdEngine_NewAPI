#pragma once

namespace fastbird{
	class IRenderer;
	typedef std::shared_ptr<IRenderer> IRendererPtr;
	typedef std::weak_ptr<IRenderer> IRendererWeakPtr;
	typedef unsigned HWND_ID;
	/** Interface for specific renderers like D3D11, OpenGL.
	This is an internal interface and not intended to used as an API
	by clients. However they can crea	Except when day want to implement their own renderer.
	Clients should use \see Renderer instead of this one.
	\ internal
	*/
	class IRenderer{
	public:
		virtual ~IRenderer();
	};
}