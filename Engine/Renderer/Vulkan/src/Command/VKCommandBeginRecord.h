#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBeginRecord : public CGfxCommandBase
{
public:
	CVKCommandBeginRecord(VkCommandBuffer vkCommandBuffer)
		: m_indexSubpass(0)
	{
		Execute(vkCommandBuffer, true);
	}
	CVKCommandBeginRecord(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{
		Execute(vkCommandBuffer, false);
	}
	virtual ~CVKCommandBeginRecord(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer, bool bMainCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RECORD, "CommandBeginRecord");
		{
			if (bMainCommandBuffer) {
				vkBeginCommandBufferPrimary(vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			}
			else {
				VkRenderPass vkRenderPass = ((CVKRenderPass*)m_ptrRenderPass.GetPointer())->GetRenderPass();
				VkFramebuffer vkFrameBuffer = ((CVKFrameBuffer*)m_ptrFrameBuffer.GetPointer())->GetFrameBuffer();
				vkBeginCommandBufferSecondary(vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, vkFrameBuffer, vkRenderPass, m_indexSubpass, VK_FALSE, 0, 0);
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
