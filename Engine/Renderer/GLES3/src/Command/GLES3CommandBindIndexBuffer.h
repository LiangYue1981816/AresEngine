#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindIndexBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandBindIndexBuffer(const CGfxIndexBufferPtr ptrIndexBuffer)
		: m_ptrIndexBuffer(ptrIndexBuffer)
	{

	}
	virtual ~CGLES3CommandBindIndexBuffer(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_ptrIndexBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_INDEXBUFFER);
		{
			((CGLES3IndexBuffer*)m_ptrIndexBuffer.GetPointer())->Bind();
		}
	}


private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;
};
