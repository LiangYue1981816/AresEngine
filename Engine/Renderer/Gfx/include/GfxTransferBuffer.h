#pragma once
#include "PreHeader.h"


class CGfxTransferBuffer : public CGfxResource
{
public:
	CGfxTransferBuffer(void)
	{

	}
	virtual ~CGfxTransferBuffer(void)
	{

	}


public:
	virtual bool CopyDataToDevice(size_t offset, size_t size, const void* data) = 0;
	virtual bool CopyDataToHost(size_t offset, size_t size, void* data) = 0;
};
