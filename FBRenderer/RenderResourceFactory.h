#pragma once
namespace fastbird{	
	class RenderResourceFactory{
		friend class Renderer;
	private:
		template <typename T>
		static std::shared_ptr<T> CreateResource(){
			return T::Create();
		}
	};
}



