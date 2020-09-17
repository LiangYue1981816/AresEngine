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
	virtual uint32_t GetBinding(void) const = 0;
	virtual uint32_t GetFormat(void) const = 0;
	virtual uint32_t GetCount(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
