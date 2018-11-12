#pragma once
#include "GfxRenderer.h"


typedef struct DrawCommand {
	int indexCount;
	int instanceCount;
	int firstIndex;
	int baseVertex;
	int reservedMustBeZero;
} DrawCommand;

class CALL_API CGfxDrawIndirectBuffer
{
public:
	CGfxDrawIndirectBuffer(size_t size)
		: m_size(size)
	{
		CGfxProfiler::IncDrawIndirectBufferSize(m_size);
	}
	virtual ~CGfxDrawIndirectBuffer(void)
	{
		CGfxProfiler::DecDrawIndirectBufferSize(m_size);
	}


public:
	virtual bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount) = 0;


protected:
	uint32_t m_size;
};
