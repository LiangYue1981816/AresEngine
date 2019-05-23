#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandEndRenderPass : public CGfxCommandBase
{
public:
	CGLES3CommandEndRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{

	}
	virtual ~CGLES3CommandEndRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrRenderPass);
		ASSERT(m_ptrFrameBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RENDERPASS, "CommandEndRenderPass");
		{

		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
};
