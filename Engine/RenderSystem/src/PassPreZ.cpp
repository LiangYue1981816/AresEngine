#include "EngineHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassPreZ::Create(GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_PREZ_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetDepthStencilAttachment(0, depthPixelFormat, 1, false, true, depth, stencil);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
	ptrRenderPass->Create();
}

void CPassPreZ::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassPreZ::CPassPreZ(CRenderSystem* pRenderSystem)
	: m_pCamera(nullptr)
	, m_pRenderSystem(pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(PASS_PREZ_NAME, ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
}

CPassPreZ::~CPassPreZ(void)
{

}

void CPassPreZ::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera  = pCamera;
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetCameraUniform()->GetUniformBuffer(), 0, m_pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassPreZ::SetOutputTexture(CGfxRenderTexturePtr ptrDepthTexture)
{
	m_ptrDepthTexture = ptrDepthTexture;

	m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrDepthTexture->GetWidth(), m_ptrDepthTexture->GetHeight(), numAttachments);
	m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrDepthTexture);
	m_ptrFrameBuffer->Create(ptrRenderPass);
}

void CPassPreZ::Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrMainCommandBuffer)
{
	if (m_pCamera == nullptr || m_pRenderSystem == nullptr) {
		return;
	}

	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();
	m_ptrDescriptorSetPass->Update();

	// Render
	GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrDepthTexture, GFX_IMAGE_LAYOUT_GENERAL);
	GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
	{
		m_pCamera->GetRenderQueue()->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass, PASS_PREZ_NAME, m_pCamera->GetCamera()->GetScissor(), m_pCamera->GetCamera()->GetViewport(), 0xffffffff);
	}
	GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
	GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrDepthTexture, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}
