#include "EngineHeader.h"
#include "RenderHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 2;
static CGfxRenderPassPtr ptrRenderPass;

void CPassForwardLighting::Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_FORWARD_LIGHTING_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(1, depthPixelFormat, 1, false, false, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void CPassForwardLighting::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassForwardLighting::CPassForwardLighting(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_NAME, STORAGE_SCENE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_SSAO_TEXTURE_NAME, UNIFORM_SSAO_TEXTURE_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_SHADOW_TEXTURE_NAME, UNIFORM_SHADOW_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_FORWARD_LIGHTING_NAME, this), ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetStorageBuffer(STORAGE_SCENE_NAME, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer(), 0, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer()->GetSize());
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
}

CPassForwardLighting::~CPassForwardLighting(void)
{

}

void CPassForwardLighting::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassForwardLighting::SetInputTexture(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrSSAOTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	CGfxSampler* pSamplerLinear = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_LINEAR, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputShadowTexture != ptrShadowTexture) {
		m_ptrInputShadowTexture = ptrShadowTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_SHADOW_TEXTURE_NAME, ptrShadowTexture, pSamplerPoint);
	}

	if (m_ptrInputSSAOTexture != ptrSSAOTexture) {
		m_ptrInputSSAOTexture = ptrSSAOTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_SSAO_TEXTURE_NAME, ptrSSAOTexture, pSamplerLinear);
	}
}

void CPassForwardLighting::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture || m_ptrOutputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrOutputColorTexture = ptrColorTexture;
		m_ptrOutputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(0, ptrColorTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(1, ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassForwardLighting::Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();
	m_ptrDescriptorSetPass->Update();

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassForwardLighting");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
		{
			m_pCamera->GetRenderQueue()->CmdDraw(taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_FORWARD_LIGHTING_NAME, m_pCamera->GetCamera()->GetScissor(), m_pCamera->GetCamera()->GetViewport(), 0xffffffff, false);
			m_pCamera->GetRenderQueue()->CmdDraw(taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_FORWARD_LIGHTING_NAME, m_pCamera->GetCamera()->GetScissor(), m_pCamera->GetCamera()->GetViewport(), 0xffffffff, true);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
