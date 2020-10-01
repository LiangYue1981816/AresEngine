#pragma once
#include "PreHeader.h"


class CGfxUniformBuffer : public CGfxResource
{
public:
	CGfxUniformBuffer(void)
	{

	}
	virtual ~CGfxUniformBuffer(void)
	{

	}


public:
	virtual size_t GetSize(void) const = 0;

public:
	virtual bool CopyDataToDevice(size_t offset, size_t size, const void* data) = 0;
};
