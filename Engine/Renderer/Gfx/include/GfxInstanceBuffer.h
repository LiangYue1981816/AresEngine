#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxInstanceBuffer
{
public:
	CGfxInstanceBuffer(uint32_t instanceFormat, uint32_t instanceBinding)
	{

	}
	virtual ~CGfxInstanceBuffer(void)
	{

	}


public:
	virtual uint32_t GetInstanceFormat(void) const = 0;
	virtual uint32_t GetInstanceCount(void) const = 0;
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t size, const void *pBuffer) = 0;
};
