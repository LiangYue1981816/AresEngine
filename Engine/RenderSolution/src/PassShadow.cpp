#include "EngineHeader.h"


CPassShadow::CPassShadow(CRenderSolution* pRenderSolution)
	: m_pRenderSolution(pRenderSolution)
{
	/*
	// CommandBuffer
	{
		m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
		m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
		m_ptrMainCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);
	}

	// DescriptorSet
	{
		CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, DESCRIPTOR_BIND_CAMERA);
		ptrDescriptorLayout->Create();

		m_ptrDescriptorSetDefaultPass = GfxRenderer()->NewDescriptorSet(DEFAULT_PASS_NAME, ptrDescriptorLayout);
		m_ptrDescriptorSetDefaultPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pRenderSolution->GetMainCameraUniformBuffer(), 0, m_pRenderSolution->GetMainCameraUniformBuffer()->GetSize());
		m_ptrDescriptorSetDefaultPass->Update();
	}

	// RenderPass and FrameBuffer
	{
		const int numAttachments = 2;
		const int numSubpasses = 1;

		m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Default"), numAttachments, numSubpasses);
		{
			m_ptrRenderPass->SetColorAttachment(0, m_pRenderSolution->GetPresentTexture(0)->GetFormat(), m_pRenderSolution->GetPresentTexture(0)->GetSamples(), true, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
			m_ptrRenderPass->SetDepthStencilAttachment(1, m_pRenderSolution->GetDepthStencilTexture(0)->GetFormat(), m_pRenderSolution->GetDepthStencilTexture(0)->GetSamples(), true, true, 1.0f, 0);

			m_ptrRenderPass->SetSubpassOutputColorReference(0, 0);
			m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
		}
		m_ptrRenderPass->Create();

		for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
			m_ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(m_pRenderSolution->GetPresentTexture(indexFrame)->GetWidth(), m_pRenderSolution->GetPresentTexture(indexFrame)->GetHeight(), numAttachments);
			{
				m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, m_pRenderSolution->GetPresentTexture(indexFrame));
				m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(1, m_pRenderSolution->GetDepthStencilTexture(indexFrame));
			}
			m_ptrFrameBuffer[indexFrame]->Create(m_ptrRenderPass);
		}
	}
	*/
}

CPassShadow::~CPassShadow(void)
{

}

void CPassShadow::Render(int indexQueue, bool bMSAA)
{

}
