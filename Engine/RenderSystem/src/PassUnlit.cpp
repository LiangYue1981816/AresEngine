#include "RenderHeader.h"


static const int indexAttachmentColor = 0;
static const int indexAttachmentDepthStencil = 1;

static const int numSubpasses = 1;
static const int numAttachments = 2;
static CGfxRenderPassPtr ptrRenderPass;

void CPassUnlit::Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_UNLIT_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentColor, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(indexAttachmentDepthStencil, depthPixelFormat, 1, false, false, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentColor);
	ptrRenderPass->SetSubpassDepthStencilReference(0, indexAttachmentDepthStencil);
	ptrRenderPass->Create();
}

void CPassUnlit::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassUnlit::CPassUnlit(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_DATA_NAME, STORAGE_SCENE_DATA_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_UNLIT_NAME, this), ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceBuffer(), 0, m_pRenderSystem->GetInstanceBuffer()->GetSize());
}

CPassUnlit::~CPassUnlit(void)
{

}

void CPassUnlit::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CPassUnlit::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture || m_ptrOutputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrOutputColorTexture  = ptrColorTexture;
		m_ptrOutputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentColor, ptrColorTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentDepthStencil, ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassUnlit::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Update DescriptorSet
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetUniformBuffer(), m_pCamera->GetUniformBufferOffset(), m_pCamera->GetUniformBufferSize());
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferOffset(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferSize());

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassDefault");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
		{
			m_pCamera->GetRenderQueue()->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_UNLIT_NAME, m_pCamera->GetScissor(), m_pCamera->GetViewport(), m_pCamera->GetZNear(), m_pCamera->GetZFar(), 0xffffffff, false);
			m_pCamera->GetRenderQueue()->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_UNLIT_NAME, m_pCamera->GetScissor(), m_pCamera->GetViewport(), m_pCamera->GetZNear(), m_pCamera->GetZFar(), 0xffffffff, true);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
