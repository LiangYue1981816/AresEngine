#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandEndRenderPass : public CGfxCommandBase
{
public:
	CVKCommandEndRenderPass(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{
		Execute();
	}
	virtual ~CVKCommandEndRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RENDERPASS, "CommandEndRenderPass");
		{
			vkCmdEndRenderPass(m_vkCommandBuffer);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
};
