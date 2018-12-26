#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxVertexBuffer
{
public:
	CGfxVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
		: m_format(vertexFormat)
		, m_count(size / GetVertexStride(vertexFormat))
		, m_size(size)
	{
		CGfxProfiler::IncVertexBufferSize(m_size);
	}
	virtual ~CGfxVertexBuffer(void)
	{
		CGfxProfiler::DecVertexBufferSize(m_size);
	}


public:
	virtual bool BufferData(size_t offset, size_t size, const void *pBuffer) = 0;

public:
	uint32_t GetVertexFormat(void) const
	{
		return m_format;
	}

	uint32_t GetVertexCount(void) const
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
};
