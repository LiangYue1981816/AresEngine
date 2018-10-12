#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandEndRenderPass : public CGfxCommandBase
{
public:
	CGfxCommandEndRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{

	}
	virtual ~CGfxCommandEndRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RENDERPASS, "CommandEndRenderPass");
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
};
