#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBeginRenderPass : public CGfxCommandBase
{
public:
	CGLES3CommandBeginRenderPass(CGfxFrameBufferPtr &ptrFrameBuffer, CGfxRenderPassPtr &ptrRenderPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{

	}
	virtual ~CGLES3CommandBeginRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RENDERPASS, "CommandBeginRenderPass");
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
};
