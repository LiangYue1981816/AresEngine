#pragma once
#include "PreHeader.h"


class CALL_API CGfxStorageBuffer : public CGfxResource
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
	virtual uint32_t GetRange(void) const = 0;
	virtual uint32_t GetOffset(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
	virtual bool BufferRange(size_t offset, size_t range) = 0;
};
