#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawIndirect : public CGfxCommandBase
{
public:
	CGLES3CommandDrawIndirect(const CGfxIndirectBufferPtr ptrIndirectBuffer)
		: m_ptrIndirectBuffer(ptrIndirectBuffer)
	{

	}
	virtual ~CGLES3CommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_ptrIndirectBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT);
		{

		}
	}


private:
	CGfxIndirectBufferPtr m_ptrIndirectBuffer;
};
