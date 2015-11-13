#include "stdafx.h"
#include "IndexBuffer.h"
#include "IPlatformIndexBuffer.h"
using namespace fastbird;
class IndexBuffer::Impl{
public:
	INDEXBUFFER_FORMAT mFormat;
	unsigned mNumIndices;
	IPlatformIndexBufferPtr mPlatformBuffer;

	//---------------------------------------------------------------------------
	Impl(unsigned numIndices, INDEXBUFFER_FORMAT format)
		: mFormat(format)
		, mNumIndices(numIndices){

	}

	bool IsReady() const{
		return mPlatformBuffer->IsReady();
	}

	void Bind() const{
		mPlatformBuffer->Bind();
	}

	MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) const{
		return mPlatformBuffer->Map(type, subResource, flag);
	}

	void Unmap() const{
		mPlatformBuffer->Unmap();
	}

	unsigned GetNumIndices() const{
		return mNumIndices;
	}

	INDEXBUFFER_FORMAT GetFormat() const{
		return mFormat;
	}

	void SetPlatformBuffer(IPlatformIndexBufferPtr buffer) {
		mPlatformBuffer = buffer;
	}

	IPlatformIndexBufferPtr GetPlatformBuffer() const{
		return mPlatformBuffer;
	}
};

//---------------------------------------------------------------------------
IndexBufferPtr IndexBuffer::Create(unsigned numIndices, INDEXBUFFER_FORMAT format){
	return IndexBufferPtr(FB_NEW(IndexBuffer)(numIndices, format), [](IndexBuffer* obj){ FB_DELETE(obj); });
}

IndexBuffer::IndexBuffer(unsigned numIndices, INDEXBUFFER_FORMAT format)
	: mImpl(new Impl(numIndices, format)){
}

bool IndexBuffer::IsReady() const{
	return mImpl->IsReady();
}

void IndexBuffer::Bind() const{
	mImpl->Bind();
}

MapData IndexBuffer::Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) const{
	return mImpl->Map(type, subResource, flag);
}

void IndexBuffer::Unmap() const{
	mImpl->Unmap();
}

unsigned IndexBuffer::GetNumIndices() const{
	return mImpl->GetNumIndices();
}

INDEXBUFFER_FORMAT IndexBuffer::GetFormat() const{
	return mImpl->GetFormat();
}

void IndexBuffer::SetPlatformBuffer(IPlatformIndexBufferPtr buffer){
	mImpl->SetPlatformBuffer(buffer);
}

IPlatformIndexBufferPtr IndexBuffer::GetPlatformBuffer(){
	return mImpl->GetPlatformBuffer();
}