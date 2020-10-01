#pragma once
#include "PreHeader.h"


class CGfxVertexBuffer : public CGfxResource
{
public:
	CGfxVertexBuffer(void)
	{

	}
	virtual ~CGfxVertexBuffer(void)
	{

	}


public:
	virtual uint32_t GetBinding(void) const = 0;
	virtual uint32_t GetFormat(void) const = 0;
	virtual uint32_t GetCount(void) const = 0;

public:
	virtual bool CopyDataToDevice(size_t offset, size_t size, const void* data) = 0;
};
