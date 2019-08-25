#include "EngineHeader.h"


static CGfxRenderPassPtr ptrRenderPass;

CPassDefault::CPassDefault(CCamera* pCamera, CRenderSystem* pRenderSystem)
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

		m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(DEFAULT_PASS_NAME, ptrDescriptorLayout);
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetCameraUniform()->GetUniformBuffer(), 0, m_pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->Update();
	}
}

CPassDefault::~CPassDefault(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

void CPassDefault::Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat, int samples)
{
	const int numSubpasses = 1;
	const int numAttachments = 2;

	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(DEFAULT_PASS_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, colorPixelFormat, samples, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(1, depthPixelFormat, samples, true, true, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void CPassDefault::Destroy(void)
{
	ptrRenderPass.Release();
}

void CPassDefault::CreateFrameBuffer(int indexFrame, CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	const int numSubpasses = 1;
	const int numAttachments = 2;

	m_ptrColorTexture[indexFrame] = ptrColorTexture;
	m_ptrDepthStencilTexture[indexFrame] = ptrDepthStencilTexture;

	m_ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(m_ptrColorTexture[indexFrame]->GetWidth(), m_ptrColorTexture[indexFrame]->GetHeight(), numAttachments);
	m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, m_ptrColorTexture[indexFrame]);
	m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(1, m_ptrDepthStencilTexture[indexFrame]);
	m_ptrFrameBuffer[indexFrame]->Create(ptrRenderPass);
}

const CGfxSemaphore* CPassDefault::Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bPresent)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	const int indexFrame = bPresent ? GfxRenderer()->GetSwapChain()->GetFrameIndex() : 0;
	const CGfxFrameBufferPtr ptrFrameBuffer = m_ptrFrameBuffer[indexFrame];
	const CGfxRenderTexturePtr ptrColorTexture = m_ptrColorTexture[indexFrame];
	const CGfxRenderTexturePtr ptrDepthStencilTexture = m_ptrDepthStencilTexture[indexFrame];
	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	{
		ptrMainCommandBuffer->Clearup();

		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
			{
				m_pCamera->GetRenderQueue()->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass, DEFAULT_PASS_NAME, m_pCamera->GetCamera()->GetScissor(), m_pCamera->GetCamera()->GetViewport(), 0xffffffff);
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrColorTexture, bPresent ? GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL : GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
	return ptrMainCommandBuffer->GetSemaphore();
}
