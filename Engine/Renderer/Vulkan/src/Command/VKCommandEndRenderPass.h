#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandEndRenderPass : public CGfxCommandBase
{
public:
	CVKCommandEndRenderPass(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandEndRenderPass(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RENDERPASS, "CommandEndRenderPass");
		{
			vkCmdEndRenderPass(vkCommandBuffer);
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
};
