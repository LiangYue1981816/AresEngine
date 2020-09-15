#pragma once
#include "PreHeader.h"


class CGfxTransferBuffer : public CGfxResource
{
public:
	CGfxTransferBuffer(size_t size)
	{

	}
	virtual ~CGfxTransferBuffer(void)
	{

	}


public:
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
