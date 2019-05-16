#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandNextSubPass : public CGfxCommandBase
{
public:
	CVKCommandNextSubPass(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{
		Execute();
	}
	virtual ~CVKCommandNextSubPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_NEXT_SUBPASS, "CommandNextSubPass");
		{
			vkCmdNextSubpass(m_vkCommandBuffer, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
