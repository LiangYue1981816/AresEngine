#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandEndPass : public CGfxCommandBase
{
public:
	CGfxCommandEndPass(const CGfxFrameBufferPtr &ptrFrameBuffer)
		: m_ptrFrameBuffer(ptrFrameBuffer)
	{

	}
	virtual ~CGfxCommandEndPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_PASS, "CommandEndPass");
		{
			if (m_ptrFrameBuffer.IsValid()) {
				m_ptrFrameBuffer->InvalidateFramebuffer();
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
};
