#pragma once
#include "PreHeader.h"


class CGfxUniformBuffer : public CGfxResource
{
public:
	CGfxUniformBuffer(size_t size)
	{

	}
	virtual ~CGfxUniformBuffer(void)
	{

	}


public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
