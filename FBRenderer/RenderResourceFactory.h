#pragma once
namespace fastbird{	
	class RenderResourceFactory{
		friend class Renderer;
	private:
		template <typename T>
		static std::shared_ptr<T> CreateResource(RendererPtr renderer){
			return std::shared_ptr<T>(FB_NEW(T), [](T* obj){ FB_DELETE(obj); });
		}
	};
}