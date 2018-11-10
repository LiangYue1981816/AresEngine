#pragma once
#include "GfxRenderer.h"


class CGfxIndexBuffer
{
public:
	CGfxIndexBuffer(uint32_t type, size_t size, bool bDynamic)
		: m_type(type)
		, m_size(size)
	{
		CGfxProfiler::IncIndexBufferSize(m_size);
	}
	virtual ~CGfxIndexBuffer(void)
	{
		CGfxProfiler::DecIndexBufferSize(m_size);
	}


public:
	virtual bool BufferData(size_t offset, size_t size, const void *pBuffer) = 0;
	virtual void Bind(void *pParam) = 0;

public:
	uint32_t GetIndexType(void) const
	{
		return m_type;
	}

	uint32_t GetIndexCount(void) const
	{
		switch (m_type) {
		case GL_UNSIGNED_BYTE:  return m_size / 1;
		case GL_UNSIGNED_SHORT: return m_size / 2;
		case GL_UNSIGNED_INT:   return m_size / 4;
		default:                return 0;
		}
	}

	uint32_t GetSize(void) const
	{
		return m_size;
	}


protected:
	uint32_t m_type;
	uint32_t m_size;
};
