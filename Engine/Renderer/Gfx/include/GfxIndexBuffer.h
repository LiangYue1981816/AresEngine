#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxIndexBuffer
{
public:
	CGfxIndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
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
	GfxIndexType GetIndexType(void) const
	{
		return m_type;
	}

	uint32_t GetIndexCount(void) const
	{
		switch (m_type) {
		case GFX_INDEX_UNSIGNED_BYTE:  return m_size / 1;
		case GFX_INDEX_UNSIGNED_SHORT: return m_size / 2;
		case GFX_INDEX_UNSIGNED_INT:   return m_size / 4;
		default:                       return 0;
		}
	}

	uint32_t GetSize(void) const
	{
		return m_size;
	}


protected:
	GfxIndexType m_type;
	uint32_t m_size;
};
