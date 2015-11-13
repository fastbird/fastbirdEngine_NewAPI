#include "stdafx.h"
#include "VertexBuffer.h"
#include "IPlatformVertexBuffer.h"
using namespace fastbird;

class VertexBuffer::Impl{
public:
	unsigned mStride;
	unsigned mNumVertices;
	IPlatformVertexBufferPtr mPlatformBuffer;
	//---------------------------------------------------------------------------
	Impl(unsigned stride, unsigned numVertices)
		: mStride(stride)
		, mNumVertices(numVertices)
	{
	}

	unsigned GetStride() const { 
		return mStride; 
	}

	unsigned GetNumVertices() const { 
		return mNumVertices; 
	}

	void SetPlatformBuffer(IPlatformVertexBufferPtr buffer){
		mPlatformBuffer = buffer;
	}

	IPlatformVertexBufferPtr GetPlatformBuffer() const{
		return mPlatformBuffer;
	}

	bool IsReady() const{
		return mPlatformBuffer->IsReady();
	}

	void Bind() const {
		mPlatformBuffer->Bind();
	}

	MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) const{
		return mPlatformBuffer->Map(type, subResource, flag);
	}

	void Unmap() const{
		mPlatformBuffer->Unmap();
	}
};

//---------------------------------------------------------------------------
VertexBufferPtr VertexBuffer::Create(unsigned stride, unsigned numVertices){
	return VertexBufferPtr(FB_NEW(VertexBuffer)(stride, numVertices), [](VertexBuffer* obj){ FB_DELETE(obj); });
}

VertexBuffer::VertexBuffer(unsigned stride, unsigned numVertices)
	: mImpl(new Impl(stride, numVertices))
{
}

void VertexBuffer::Bind() const{
	mImpl->Bind();
}

MapData VertexBuffer::Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) const{
	return mImpl->Map(type, subResource, flag);
}

void VertexBuffer::Unmap() const{
	mImpl->Unmap();
}

unsigned VertexBuffer::GetStride() const{
	return mImpl->GetStride();
}

unsigned VertexBuffer::GetNumVertices() const{
	return mImpl->GetNumVertices();
}

void VertexBuffer::SetPlatformBuffer(IPlatformVertexBufferPtr buffer){
	mImpl->SetPlatformBuffer(buffer);
}

IPlatformVertexBufferPtr VertexBuffer::GetPlatformBuffer() const{
	return mImpl->GetPlatformBuffer();
}