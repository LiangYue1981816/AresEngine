#include "EngineHeader.h"


CPassForwardLighting::CPassForwardLighting(CCamera* pCamera, CRenderSystem* pRenderSystem)
	: m_pCamera(pCamera)
	, m_pRenderSystem(pRenderSystem)
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
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, DESCRIPTOR_BIND_ENGINE);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, DESCRIPTOR_BIND_CAMERA);
		ptrDescriptorLayout->Create();

		m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(FORWARD_LIGHTING_PASS_NAME, ptrDescriptorLayout);
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetCameraUniform()->GetUniformBuffer(), 0, m_pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->Update();
	}
}

CPassForwardLighting::~CPassForwardLighting(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

void CPassForwardLighting::CreateFrameBuffer(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	const int numSubpasses = 1;
	const int numAttachments = 2;

	m_ptrColorTexture = ptrColorTexture;
	m_ptrDepthStencilTexture = ptrDepthStencilTexture;

	// RenderPass
	{
		const int stencil = 0;
		const float depth = 1.0f;
		const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

		m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("ForwardLighting"), numAttachments, numSubpasses);
		m_ptrRenderPass->SetColorAttachment(0, m_ptrColorTexture->GetFormat(), m_ptrColorTexture->GetSamples(), false, false, true, color[0], color[1], color[2], color[3]);
		m_ptrRenderPass->SetDepthStencilAttachment(1, m_ptrDepthStencilTexture->GetFormat(), m_ptrDepthStencilTexture->GetSamples(), true, true, depth, stencil);
		m_ptrRenderPass->SetSubpassOutputColorReference(0, 0);
		m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
		m_ptrRenderPass->Create();
	}

	// FrameBuffer
	{
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrColorTexture->GetWidth(), m_ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrColorTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(1, m_ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(m_ptrRenderPass);
	}
}

void CPassForwardLighting::Update(void)
{

}

void CPassForwardLighting::Render(int indexQueue, const CGfxSemaphore* pWaitSemaphore)
{
	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	{
		ptrMainCommandBuffer->Clearup();

		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrColorTexture, GFX_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);

			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass);
			{
				m_pCamera->GetRenderQueue()->CmdDraw(indexQueue, ptrMainCommandBuffer, m_ptrDescriptorSetPass, FORWARD_LIGHTING_PASS_NAME, m_pCamera->GetCamera()->GetScissor(), m_pCamera->GetCamera()->GetViewport(), 0xffffffff);
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
}

const CGfxSemaphore* CPassForwardLighting::GetSemaphore(void) const
{
	return m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()]->GetSemaphore();
}
