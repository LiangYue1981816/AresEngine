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
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrFrameBuffer);
		ASSERT(m_ptrRenderPass);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RENDERPASS, "CommandBeginRenderPass");
		{
			eastl::vector<VkClearValue> clearValues;
			for (int indexAttachment = 0; indexAttachment < (int)m_ptrRenderPass->GetAttachmentCount(); indexAttachment++) {
				if (const AttachmentInformation* pAttachment = m_ptrRenderPass->GetAttachment(indexAttachment)) {
					if (CGfxHelper::IsFormatColor(pAttachment->format)) {
						VkClearValue value = {};
						value.color.float32[0] = pAttachment->clearValue.color[0];
						value.color.float32[1] = pAttachment->clearValue.color[1];
						value.color.float32[2] = pAttachment->clearValue.color[2];
						value.color.float32[3] = pAttachment->clearValue.color[3];
						clearValues.emplace_back(value);
					}

					if (CGfxHelper::IsFormatDepthOrStencil(pAttachment->format)) {
						VkClearValue value = {};
						value.depthStencil.depth = pAttachment->clearValue.depth;
						value.depthStencil.stencil = pAttachment->clearValue.stencil;
						clearValues.emplace_back(value);
					}
				}
			}

			VkRenderPassBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.renderPass = ((CVKRenderPass*)m_ptrRenderPass.GetPointer())->GetRenderPass();
			beginInfo.framebuffer = ((CVKFrameBuffer*)m_ptrFrameBuffer.GetPointer())->GetFrameBuffer();
			beginInfo.renderArea.offset.x = 0;
			beginInfo.renderArea.offset.y = 0;
			beginInfo.renderArea.extent.width = m_ptrFrameBuffer->GetWidth();
			beginInfo.renderArea.extent.height = m_ptrFrameBuffer->GetHeight();
			beginInfo.clearValueCount = clearValues.size();
			beginInfo.pClearValues = clearValues.data();
			vkCmdBeginRenderPass(m_vkCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
