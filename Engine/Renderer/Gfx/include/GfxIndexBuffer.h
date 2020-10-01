#pragma once
#include "PreHeader.h"


class CGfxIndexBuffer : public CGfxResource
{
public:
	CGfxIndexBuffer(void)
	{

	}
	virtual ~CGfxIndexBuffer(void)
	{

	}


public:
	virtual GfxIndexType GetType(void) const = 0;
	virtual uint32_t GetCount(void) const = 0;

public:
	virtual bool CopyDataToDevice(size_t offset, size_t size, const void* data) = 0;
};
