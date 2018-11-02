#pragma once
#include "GfxRenderer.h"


class CGfxUniformBuffer
{
public:
	CGfxUniformBuffer(size_t size, bool bDynamic);
	virtual ~CGfxUniformBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer) const;

public:
	uint32_t GetBuffer(void) const;
	uint32_t GetSize(void) const;


private:
	uint32_t m_buffer;
	uint32_t m_size;
};
