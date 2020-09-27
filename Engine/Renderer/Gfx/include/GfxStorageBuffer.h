#pragma once
#include "PreHeader.h"


class CGfxStorageBuffer : public CGfxResource
{
public:
	CGfxStorageBuffer(void)
	{

	}
	virtual ~CGfxStorageBuffer(void)
	{

	}


public:
	virtual size_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
