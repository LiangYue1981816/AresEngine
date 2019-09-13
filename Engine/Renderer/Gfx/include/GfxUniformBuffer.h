#pragma once
#include "PreHeader.h"


class CALL_API CGfxUniformBuffer : public CGfxResource
{
public:
	CGfxUniformBuffer(size_t size)
	{

	}
	virtual ~CGfxUniformBuffer(void)
	{

	}


public:
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
