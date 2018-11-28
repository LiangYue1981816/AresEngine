#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformBuffer : public CGfxResource
{
public:
	CGfxUniformBuffer(size_t size, bool bDynamic)
		: m_size(size)
	{
		CGfxProfiler::IncUniformBufferSize(m_size);
	}
	virtual ~CGfxUniformBuffer(void)
	{
		CGfxProfiler::DecUniformBufferSize(m_size);
	}


public:
	virtual bool BufferData(size_t offset, size_t size, const void *pBuffer) = 0;

public:
	uint32_t GetSize(void) const
	{
		return m_size;
	}


protected:
	uint32_t m_size;
};
