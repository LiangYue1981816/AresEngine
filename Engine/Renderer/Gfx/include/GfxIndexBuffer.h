#pragma once
#include "PreHeader.h"


class CALL_API CGfxIndexBuffer : public CGfxResource
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

class CALL_API CGfxMultiIndexBuffer : public CGfxResource
{
public:
	CGfxMultiIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, int count)
	{

	}
	virtual ~CGfxMultiIndexBuffer(void)
	{

	}


public:
	virtual bool SetIndex(int index) = 0;

public:
	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
