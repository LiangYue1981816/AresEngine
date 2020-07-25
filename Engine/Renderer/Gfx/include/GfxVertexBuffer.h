#pragma once
#include "PreHeader.h"


class CGfxVertexBuffer : public CGfxResource
{
public:
	CGfxVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic)
	{

	}
	virtual ~CGfxVertexBuffer(void)
	{

	}


public:
	virtual uint32_t GetVertexBinding(void) const = 0;
	virtual uint32_t GetVertexFormat(void) const = 0;
	virtual uint32_t GetVertexCount(void) const = 0;
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};

class CGfxMultiVertexBuffer : public CGfxResource
{
public:
	CGfxMultiVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, int count)
	{

	}
	virtual ~CGfxMultiVertexBuffer(void)
	{

	}


public:
	virtual CGfxVertexBuffer* GetBuffer(int index) const = 0;
};
