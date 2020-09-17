#pragma once
#include "PreHeader.h"


class CGfxInstanceBuffer : public CGfxResource
{
public:
	CGfxInstanceBuffer(uint32_t instanceFormat, int instanceBinding)
	{

	}
	virtual ~CGfxInstanceBuffer(void)
	{

	}


public:
	virtual uint32_t GetInstanceBinding(void) const = 0;
	virtual uint32_t GetInstanceFormat(void) const = 0;
	virtual uint32_t GetInstanceCount(void) const = 0;

public:
	virtual bool BufferData(size_t size, const void* data) = 0;
};
