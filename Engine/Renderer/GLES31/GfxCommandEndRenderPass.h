#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandEndRenderPass : public CGfxCommandBase
{
public:
	CGfxCommandEndRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer)
		: m_ptrFrameBuffer(ptrFrameBuffer)
	{

	}
	virtual ~CGfxCommandEndRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RENDERPASS, "CommandEndRenderPass");
		{
			if (m_ptrFrameBuffer.IsValid()) {
				m_ptrFrameBuffer->InvalidateFramebuffer();
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
};
