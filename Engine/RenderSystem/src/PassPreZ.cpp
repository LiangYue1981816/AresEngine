#include "EngineHeader.h"
#include "RenderHeader.h"


static const int indexAttachmentDepthStencil = 0;

static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassPreZ::Create(GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_PREZ_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetDepthStencilAttachment(indexAttachmentDepthStencil, depthPixelFormat, 1, false, true, depth, stencil);
	ptrRenderPass->SetSubpassDepthStencilReference(0, indexAttachmentDepthStencil);
	ptrRenderPass->Create();
}

void CPassPreZ::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassPreZ::CPassPreZ(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_DATA_NAME, STORAGE_SCENE_DATA_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_PREZ_NAME, this), ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniformBuffer(), 0, m_pRenderSystem->GetEngineUniformBuffer()->GetSize());
	m_ptrDescriptorSetPass->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceBuffer(), 0, m_pRenderSystem->GetInstanceBuffer()->GetSize());
}

CPassPreZ::~CPassPreZ(void)
{

}

void CPassPreZ::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetUniformBuffer(), 0, pCamera->GetUniformBuffer()->GetSize());
	}
}

void CPassPreZ::SetOutputTexture(CGfxRenderTexturePtr ptrDepthTexture)
{
	if (m_ptrOutputDepthTexture != ptrDepthTexture) {
		m_ptrOutputDepthTexture = ptrDepthTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrDepthTexture->GetWidth(), ptrDepthTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentDepthStencil, ptrDepthTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassPreZ::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassPreZ");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
		{
			m_pCamera->GetRenderQueue()->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_PREZ_NAME, m_pCamera->GetScissor(), m_pCamera->GetViewport(), m_pCamera->GetZNear(), m_pCamera->GetZFar(), 0xffffffff, false);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthTexture, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
