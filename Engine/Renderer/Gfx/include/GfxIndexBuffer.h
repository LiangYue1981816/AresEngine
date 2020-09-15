#pragma once
#include "PreHeader.h"


class CGfxIndexBuffer : public CGfxResource
{
public:
	CGfxIndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
	{

	}
	virtual ~CGfxIndexBuffer(void)
	{

	}


public:
	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
