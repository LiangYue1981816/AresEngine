#pragma once
#include "PreHeader.h"


class CGfxStorageBuffer : public CGfxResource
{
public:
	CGfxStorageBuffer(size_t size)
	{

	}
	virtual ~CGfxStorageBuffer(void)
	{

	}


public:
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
