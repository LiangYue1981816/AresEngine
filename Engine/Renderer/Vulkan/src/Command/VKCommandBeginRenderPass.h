#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBeginRenderPass : public CGfxCommandBase
{
public:
	CVKCommandBeginRenderPass(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
	{
		Execute();
	}
	virtual ~CVKCommandBeginRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RENDERPASS, "CommandBeginRenderPass");
		{
			VkRenderPassBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.renderPass = ((CVKRenderPass*)m_ptrRenderPass.GetPointer())->GetRenderPass();
			beginInfo.framebuffer = ((CVKFrameBuffer*)m_ptrFrameBuffer.GetPointer())->GetFrameBuffer();
			beginInfo.renderArea.offset.x = 0;
			beginInfo.renderArea.offset.y = 0;
			beginInfo.renderArea.extent.width = m_ptrFrameBuffer->GetWidth();
			beginInfo.renderArea.extent.height = m_ptrFrameBuffer->GetHeight();
			beginInfo.clearValueCount = 0;
			beginInfo.pClearValues = nullptr;
			vkCmdBeginRenderPass(m_vkCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
};
