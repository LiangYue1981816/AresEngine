#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBeginRecord : public CGfxCommandBase
{
public:
	CVKCommandBeginRecord(VkCommandBuffer vkCommandBuffer)
		: m_bMainCommandBuffer(true)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_indexSubpass(0)
	{
		Execute();
	}
	CVKCommandBeginRecord(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_bMainCommandBuffer(false)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{
		Execute();
	}
	virtual ~CVKCommandBeginRecord(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RECORD, "CommandBeginRecord");
		{
			if (m_bMainCommandBuffer) {
				vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			}
			else {
				VkRenderPass vkRenderPass = ((CVKRenderPass*)m_ptrRenderPass.GetPointer())->GetRenderPass();
				VkFramebuffer vkFrameBuffer = ((CVKFrameBuffer*)m_ptrFrameBuffer.GetPointer())->GetFrameBuffer();
				vkBeginCommandBufferSecondary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, vkFrameBuffer, vkRenderPass, m_indexSubpass, VK_FALSE, 0, 0);
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;

private:
	bool m_bMainCommandBuffer;
	VkCommandBuffer m_vkCommandBuffer;
};
