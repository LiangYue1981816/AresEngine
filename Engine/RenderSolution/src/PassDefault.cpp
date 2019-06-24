#include "EngineHeader.h"


CPassDefault::CPassDefault(CRenderSolution* pRenderSolution)
	: m_pRenderSolution(pRenderSolution)
{
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
		m_ptrDescriptorSetDefaultPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pRenderSolution->GetMainCameraUniform()->GetUniformBuffer(), 0, m_pRenderSolution->GetMainCameraUniform()->GetUniformBuffer()->GetSize());
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

	// RenderPass and FrameBuffer with MSAA
	/*
	{
		const int numAttachments = 3;
		const int numSubpasses = 1;

		m_ptrRenderPassMSAA = GfxRenderer()->NewRenderPass(HashValue("DefaultMSAA"), numAttachments, numSubpasses);
		{
			m_ptrRenderPassMSAA->SetColorAttachment(0, m_pRenderSolution->GetPresentTexture(0)->GetFormat(), m_pRenderSolution->GetPresentTexture(0)->GetSamples(), true, false, false, 0.2f, 0.2f, 0.2f, 0.0f);
			m_ptrRenderPassMSAA->SetColorAttachment(1, m_pRenderSolution->GetColorTextureMSAA(0)->GetFormat(), m_pRenderSolution->GetColorTextureMSAA(0)->GetSamples(), false, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
			m_ptrRenderPassMSAA->SetDepthStencilAttachment(2, m_pRenderSolution->GetDepthStencilTextureMSAA(0)->GetFormat(), m_pRenderSolution->GetDepthStencilTextureMSAA(0)->GetSamples(), true, true, 1.0f, 0);

			m_ptrRenderPassMSAA->SetSubpassResolveReference(0, 0);
			m_ptrRenderPassMSAA->SetSubpassOutputColorReference(0, 1);
			m_ptrRenderPassMSAA->SetSubpassOutputDepthStencilReference(0, 2);
		}
		m_ptrRenderPassMSAA->Create();

		for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
			m_ptrFrameBufferMSAA[indexFrame] = GfxRenderer()->NewFrameBuffer(m_pRenderSolution->GetPresentTexture(indexFrame)->GetWidth(), m_pRenderSolution->GetPresentTexture(indexFrame)->GetHeight(), numAttachments);
			{
				m_ptrFrameBufferMSAA[indexFrame]->SetAttachmentTexture(0, m_pRenderSolution->GetPresentTexture(indexFrame));
				m_ptrFrameBufferMSAA[indexFrame]->SetAttachmentTexture(1, m_pRenderSolution->GetColorTextureMSAA(indexFrame));
				m_ptrFrameBufferMSAA[indexFrame]->SetAttachmentTexture(2, m_pRenderSolution->GetDepthStencilTextureMSAA(indexFrame));
			}
			m_ptrFrameBufferMSAA[indexFrame]->Create(m_ptrRenderPassMSAA);
		}
	}
	*/
}

CPassDefault::~CPassDefault(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

void CPassDefault::Update(void)
{

}

void CPassDefault::Render(int indexQueue, bool bMSAA)
{
	const int indexFrame = GfxRenderer()->GetSwapChain()->GetFrameIndex();

	const CGfxRenderPassPtr ptrRenderPass = bMSAA ? m_ptrRenderPassMSAA : m_ptrRenderPass;
	const CGfxFrameBufferPtr ptrFrameBuffer = bMSAA ? m_ptrFrameBufferMSAA[indexFrame] : m_ptrFrameBuffer[indexFrame];
	const CGfxRenderTexturePtr ptrPresentTexture = bMSAA ? m_pRenderSolution->GetPresentTexture(indexFrame) : m_pRenderSolution->GetPresentTexture(indexFrame);
	const CGfxRenderTexturePtr ptrColorTexture = bMSAA ? m_pRenderSolution->GetColorTextureMSAA(indexFrame) : m_pRenderSolution->GetPresentTexture(indexFrame);
	const CGfxRenderTexturePtr ptrDepthStencilTexture = bMSAA ? m_pRenderSolution->GetDepthStencilTextureMSAA(indexFrame) : m_pRenderSolution->GetDepthStencilTexture(indexFrame);

	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[indexFrame];
	{
		ptrMainCommandBuffer->Clearup();
		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			if (bMSAA) {
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrPresentTexture, GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL);
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
			}
			else {
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrPresentTexture, GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL);
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
			}

			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
			{
				m_pRenderSolution->GetMainCameraQueue()->CmdDraw(indexQueue, ptrMainCommandBuffer, m_ptrDescriptorSetDefaultPass, DEFAULT_PASS_NAME, m_pRenderSolution->GetMainCamera()->GetScissor(), m_pRenderSolution->GetMainCamera()->GetViewport(), 0xffffffff);
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer);
}
