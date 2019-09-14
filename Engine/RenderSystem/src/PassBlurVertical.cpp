#include "RenderHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassBlurVertical::Create(GfxPixelFormat colorPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_BLUR_VERTICAL_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->Create();
}

void CPassBlurVertical::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassBlurVertical::CPassBlurVertical(CRenderSystem* pRenderSystem)
	: CPassBlit(PASS_BLUR_VERTICAL_MATERIAL_NAME, pRenderSystem)
	, m_range(1.0f)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_COLOR_TEXTURE_NAME, UNIFORM_COLOR_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(PASS_BLUR_VERTICAL_NAME, ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
}

CPassBlurVertical::~CPassBlurVertical(void)
{

}

void CPassBlurVertical::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_pCamera = pCamera;
	}
}

void CPassBlurVertical::SetInputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	CGfxSampler* pSamplerLinear = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_LINEAR, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputColorTexture != ptrColorTexture) {
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_COLOR_TEXTURE_NAME, ptrColorTexture, pSamplerLinear);
		m_ptrInputColorTexture = ptrColorTexture;
	}
}

void CPassBlurVertical::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture) {
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(0, ptrColorTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
		m_ptrOutputColorTexture = ptrColorTexture;
	}
}

void CPassBlurVertical::Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrMainCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();
	m_ptrDescriptorSetPass->Update();

	// Render
	GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
	GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
	{
		const float w = m_ptrFrameBuffer->GetWidth();
		const float h = m_ptrFrameBuffer->GetHeight();
		const glm::vec4 scissor = glm::vec4(0.0, 0.0, w, h);
		const glm::vec4 viewport = glm::vec4(0.0, 0.0, w, h);

		m_pRenderQueue->CmdDraw(&taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass, PASS_BLUR_VERTICAL_NAME, scissor, viewport, 0xffffffff);
	}
	GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
	GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
}

void CPassBlurVertical::RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer)
{
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.range"), m_range);
}