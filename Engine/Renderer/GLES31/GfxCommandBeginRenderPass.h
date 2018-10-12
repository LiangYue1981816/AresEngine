#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBeginRenderPass : public CGfxCommandBase
{
public:
	CGfxCommandBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{

	}
	virtual ~CGfxCommandBeginRenderPass(void)
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
