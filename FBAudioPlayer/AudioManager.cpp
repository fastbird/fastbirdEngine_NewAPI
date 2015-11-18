#include "stdafx.h"
#include "AudioManager.h"

using namespace fastbird;

class AudioManager::Impl{
public:
	ALCdevice* mDevice;
	ALCcontext* mContext;
	static LPALGETSOURCEDVSOFT alGetSourcedvSOFT;

	//---------------------------------------------------------------------------
	Impl()
		: mDevice(0)
		, mContext(0)
	{
	}

	~Impl(){
		if (mDevice)
			Deinit();
	}

	bool Init(){
		mDevice = alcOpenDevice(NULL);
		if (!mDevice){
			Error("No sound device found!");
			return false;
		}

		mContext = alcCreateContext(mDevice, NULL);
		if (!mContext){
			Error("Cannot create the audio context!");
			return false;
		}

		if (alcMakeContextCurrent(mContext) == ALC_FALSE)
		{
			Error("Could not make context current!\n");
			return false;
		}

		if (!alIsExtensionPresent("AL_SOFT_source_length"))
		{
			Error("Required AL_SOFT_source_length not supported!\n");
			return false;
		}

		if (!alIsExtensionPresent("AL_SOFT_source_latency"))
			Error("AL_SOFT_source_latency not supported, audio may be a bit laggy.");
		else
		{
			alGetSourcedvSOFT = (LPALGETSOURCEDVSOFT)alGetProcAddress("alGetSourcedvSOFT");
		}
		Log("OpenAL initialized!");
		return true;
	}
	void Deinit(){
		alcMakeContextCurrent(NULL);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
		mDevice = 0;
	}
};
LPALGETSOURCEDVSOFT AudioManager::Impl::alGetSourcedvSOFT = 0;

//---------------------------------------------------------------------------
IMPLEMENT_STATIC_CREATE(AudioManager);
AudioManager::AudioManager()
	: mImpl(new Impl)
{
}

bool AudioManager::Init(){
	return mImpl->Init();
}

void AudioManager::Deinit(){
	mImpl->Deinit();
}

