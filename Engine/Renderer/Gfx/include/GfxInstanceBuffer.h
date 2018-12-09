#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxInstanceBuffer
{
public:
	CGfxInstanceBuffer(uint32_t binding, uint32_t instanceFormat)
		: m_format(instanceFormat)
		, m_count(0)
		, m_size(INSTANCE_BUFFER_SIZE)
	{
		CGfxProfiler::IncInstanceBufferSize(m_size);
	}
	virtual ~CGfxInstanceBuffer(void)
	{
		CGfxProfiler::DecInstanceBufferSize(m_size);
	}


public:
	virtual bool BufferData(size_t size, const void *pBuffer) = 0;
	virtual void Bind(void *pParam) = 0;

public:
	uint32_t GetInstanceFormat(void) const
	{
		return m_format;
	}

	uint32_t GetInstanceCount(void) const
	{
		return m_count;
	}

	uint32_t GetSize(void) const
	{
		return m_size;
	}


protected:
	uint32_t m_format;
	uint32_t m_count;
	uint32_t m_size;

protected:
	static const int INSTANCE_BUFFER_SIZE = 64;
};
