#include "RenderHeader.h"


static const int indexAttachmentColor = 0;

static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassBlurHorizontal::Create(GfxPixelFormat colorPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_BLUR_HORIZONTAL_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentColor, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentColor);
	ptrRenderPass->Create();
}

void CPassBlurHorizontal::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassBlurHorizontal::CPassBlurHorizontal(CRenderSystem* pRenderSystem)
	: CPassBlit("PassBlurHorizontal.material", pRenderSystem)
	, m_range(1.0f)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_COLOR_TEXTURE_NAME, UNIFORM_COLOR_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_BLUR_HORIZONTAL_NAME, this), ptrDescriptorLayout);
}

CPassBlurHorizontal::~CPassBlurHorizontal(void)
{

}

void CPassBlurHorizontal::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CPassBlurHorizontal::SetInputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	CGfxSampler* pSamplerLinear = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_LINEAR, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputColorTexture != ptrColorTexture) {
		m_ptrInputColorTexture  = ptrColorTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_COLOR_TEXTURE_NAME, ptrColorTexture, pSamplerLinear);
	}
}

void CPassBlurHorizontal::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture) {
		m_ptrOutputColorTexture  = ptrColorTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentColor, ptrColorTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassBlurHorizontal::SetParamRange(float range)
{
	m_range = range;
}

void CPassBlurHorizontal::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Update DescriptorSet
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetUniformBuffer(), m_pCamera->GetUniformBufferOffset(), m_pCamera->GetUniformBufferSize());
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferOffset(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferSize());

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassBlurHorizontal");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
		{
			const float w = m_ptrFrameBuffer->GetWidth();
			const float h = m_ptrFrameBuffer->GetHeight();
			const glm::vec4 scissor = glm::vec4(0.0, 0.0, w, h);
			const glm::vec4 viewport = glm::vec4(0.0, 0.0, w, h);
			const float znear = 0.0f;
			const float zfar = 1.0f;

			m_pRenderQueue->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_BLUR_HORIZONTAL_NAME, scissor, viewport, znear, zfar, 0xffffffff, false);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}

void CPassBlurHorizontal::RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer)
{
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.range"), m_range);
}
