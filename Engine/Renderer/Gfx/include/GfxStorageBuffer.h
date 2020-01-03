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

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};

class CALL_API CGfxMultiStorageBuffer : public CGfxResource
{
public:
	CGfxMultiStorageBuffer(size_t size, int count)
	{

	}
	virtual ~CGfxMultiStorageBuffer(void)
	{

	}


public:
	virtual void SetBufferIndex(int index) = 0;

public:
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(size_t offset, size_t size, const void* data) = 0;
};
