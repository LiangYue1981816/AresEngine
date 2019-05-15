#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandNextSubPass : public CGfxCommandBase
{
public:
	CVKCommandNextSubPass(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandNextSubPass(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_NEXT_SUBPASS, "CommandNextSubPass");
		{
			vkCmdNextSubpass(vkCommandBuffer, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
