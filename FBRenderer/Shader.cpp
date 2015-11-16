#include "stdafx.h"
#include "Shader.h"
#include "IPlatformShader.h"
#include "Renderer.h"
#include "FBCommonHeaders/Helpers.h"
#include "FBTimer/Profiler.h"
#include "FBStringLib/StringLib.h"
using namespace fastbird;

static std::vector<ShaderWeakPtr> sAllShaders;
ShaderPtr GetShaderFromExistings(IPlatformShaderPtr platformTexture) {
	for (auto it = sAllShaders.begin(); it != sAllShaders.end();){
		auto shader = it->lock();
		if (shader){
			++it;
			if (shader->GetPlatformShader() == platformTexture){
				return shader;
			}
		}
		else{
			it = sAllShaders.erase(it);
		}
	}
}

class Shader::Impl{
public:
	IPlatformShaderPtr mPlatformShader;
	SHADER_DEFINES mDefines;
	std::set<std::string> mIncludeFiles;
	
	//---------------------------------------------------------------------------
	Impl()
	{
	}

	void Bind(){
		if (mPlatformShader)
			mPlatformShader->Bind();
	}
	bool IsValid() const{
		if (mPlatformShader)
			return mPlatformShader->IsValid();
		return false;
	}

	bool GetCompileFailed() const{
		if (mPlatformShader)
			return mPlatformShader->GetCompileFailed();

		return true;
	}

	void* GetVSByteCode(unsigned& size) const{
		if (mPlatformShader)
			return mPlatformShader->GetVSByteCode(size);

		size = 0;
		return 0;
	}

	const char* GetName() const { 
		return mPlatformShader->GetName();
	}
	void SetShaderDefines(const SHADER_DEFINES& defines){
		mDefines = defines;
		std::sort(mDefines.begin(), mDefines.end());
		if (mPlatformShader)
			mPlatformShader->SetShaderDefines(defines);
	}
	const SHADER_DEFINES& GetShaderDefines() const { 
		return mDefines; 
	}

	void ApplyShaderDefines(){
		if (mPlatformShader)
			mPlatformShader->ApplyShaderDefines();
	}

	int GetBindingShaders() const{
		if (mPlatformShader){
			return mPlatformShader->GetBindingShaders();
		}
		return 0;
	}

	void SetDebugName(const char* debugName){
		if (mPlatformShader)
			mPlatformShader->SetDebugName(debugName);
	}

	bool CheckIncludes(const char* shaderHeaderFile) const{
		return mPlatformShader ? mPlatformShader->CheckIncludes(shaderHeaderFile) : false;
	}

	void SetPlatformShader(IPlatformShaderPtr shader){
		mPlatformShader = shader;		
	}

	IPlatformShaderPtr GetPlatformShader() const{
		return mPlatformShader;
	}
};

//----------------------------------------------------------------------------
void Shader::ReloadShader(const char* name)
{
	ReloadShader(name, SHADER_DEFINES());
}

//----------------------------------------------------------------------------
void Shader::ReloadShader(const char* filepath, const SHADER_DEFINES& shaderDefines)
{
	Profiler profile("Reloading Shaders");
	std::string path = filepath;
	ToLowerCase(path);
	auto renderer = Renderer::GetInstance();
	auto it = sAllShaders.begin(), itEnd = sAllShaders.end();
	for (; it != itEnd; it++)
	{
		auto shader = it->lock();
		if (shader && shader->mImpl->mPlatformShader){
			if (strcmp(path.c_str(), shader->GetName()) == 0 || shader->mImpl->mPlatformShader->CheckIncludes(path))
			{
				if (!shaderDefines.empty() || shader->GetShaderDefines() == shaderDefines){
					auto failed = renderer->ReloadShader(shader);
					if (failed){
						Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to reload a shader(%s)", path).c_str());
					}
				}
			}
		}
	}
}

ShaderPtr Shader::Create(){
	auto shader = ShaderPtr(FB_NEW(Shader), [](Shader* obj){ FB_DELETE(obj); });
	sAllShaders.push_back(shader);
	return shader;
}

Shader::Shader()
	: mImpl(new Impl)
{	
}

Shader::~Shader(){
	auto itEnd = sAllShaders.end();
	for (auto it = sAllShaders.begin(); it != itEnd; it++){
		if (it->expired()){
			sAllShaders.erase(it);
			return;
		}
	}
}

void Shader::Bind(){
	mImpl->Bind();
}

bool Shader::IsValid() const{
	return mImpl->IsValid();
}

bool Shader::GetCompileFailed() const{
	return mImpl->GetCompileFailed();
}

void* Shader::GetVSByteCode(unsigned& size) const{
	return mImpl->GetVSByteCode(size);
}

const char* Shader::GetName() const{
	return mImpl->GetName();
}

void Shader::SetShaderDefines(const SHADER_DEFINES& defines){
	return mImpl->SetShaderDefines(defines);
}


const SHADER_DEFINES& Shader::GetShaderDefines() const{
	return mImpl->GetShaderDefines();
}

void Shader::ApplyShaderDefines(){
	mImpl->ApplyShaderDefines();
}

int Shader::GetBindingShaders() const{
	return mImpl->GetBindingShaders();
}

void Shader::SetDebugName(const char* debugName){
	mImpl->SetDebugName(debugName);
}

bool Shader::CheckIncludes(const char* shaderHeaderFile) const{
	return mImpl->CheckIncludes(shaderHeaderFile);
}

void Shader::SetPlatformShader(IPlatformShaderPtr shader){
	mImpl->SetPlatformShader(shader);
}

IPlatformShaderPtr Shader::GetPlatformShader() const{
	return mImpl->GetPlatformShader();
}