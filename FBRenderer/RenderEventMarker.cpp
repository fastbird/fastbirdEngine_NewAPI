#include "stdafx.h"
#include "RenderEventMarker.h"
#include "Renderer.h"
using namespace fastbird;

RenderEventMarker::RenderEventMarker(const char* name){
	auto renderer = Renderer::GetInstance();
	if (renderer){
		renderer->BeginEvent(name);
	}
}

RenderEventMarker::~RenderEventMarker(){
	auto renderer = Renderer::GetInstance();
	if (renderer){
		renderer->EndEvent();
	}
}