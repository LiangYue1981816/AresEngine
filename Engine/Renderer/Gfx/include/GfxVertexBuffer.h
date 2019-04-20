#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxVertexBuffer
{
public:
	CGfxVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	{

	}
	virtual ~CGfxVertexBuffer(void)
	{

	}


public:
	virtual uint32_t GetVertexFormat(void) const = 0;
	virtual uint32_t GetVertexCount(void) const = 0;
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* pBuffer) = 0;
};
