#pragma once
#include "GfxRenderer.h"


typedef struct DrawCommand {
	int indexCount = 0;
	int instanceCount = 0;
	int firstIndex = 0;
	int baseVertex = 0;
	int reservedMustBeZero = 0;
} DrawCommand;

class CALL_API CGfxDrawIndirectBuffer
{
protected:
	CGfxDrawIndirectBuffer(uint32_t count)
		: m_size(count * sizeof(DrawCommand))
		, m_count(count)
	{
		CGfxProfiler::IncDrawIndirectBufferSize(m_size);
	}
	virtual ~CGfxDrawIndirectBuffer(void)
	{
		CGfxProfiler::DecDrawIndirectBufferSize(m_size);
	}


public:
	virtual bool BufferData(size_t offset, size_t size, const void *pBuffer) = 0;
	virtual bool BufferData(int indexDraw, int instanceCount) = 0;
	virtual bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount) = 0;

public:
	uint32_t GetDrawCommandCount(void) const
	{
		return m_count;
	}

	uint32_t GetDrawCommandOffset(int indexDraw) const
	{
		return indexDraw * sizeof(DrawCommand);
	}

	uint32_t GetSize(void) const
	{
		return m_size;
	}


protected:
	uint32_t m_size;
	uint32_t m_count;
};
