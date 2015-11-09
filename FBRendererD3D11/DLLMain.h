#pragma once
namespace fastbird {
	class IRenderer;
	typedef std::shared_ptr<IRenderer> IRendererPtr;
	typedef std::weak_ptr<IRenderer> IRendererWeakPtr;
}
extern "C"{
	fastbird::IRendererPtr Create();
}