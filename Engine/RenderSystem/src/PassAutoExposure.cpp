#include "EngineHeader.h"
#include "RenderHeader.h"


static const int indexAttachmentColor = 0;

static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassAutoExposure::Create(GfxPixelFormat colorPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_AUTO_EXPOSURE_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentColor);
	ptrRenderPass->Create();
}

void CPassAutoExposure::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassAutoExposure::CPassAutoExposure(CRenderSystem* pRenderSystem)
	: CPassBlit("PassAutoExposure.material", pRenderSystem)
	, m_lower(0.10f)
	, m_upper(0.90f)
	, m_luminance(0.25f)
	, m_minScaleLuminance(0.20f)
	, m_maxScaleLuminance(2.00f)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_COLOR_TEXTURE_NAME, UNIFORM_COLOR_TEXTURE_BIND);
	ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_HISTOGRAM_DATA_NAME, STORAGE_HISTOGRAM_DATA_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_AUTO_EXPOSURE_NAME, this), ptrDescriptorLayout);
}

CPassAutoExposure::~CPassAutoExposure(void)
{

}

void CPassAutoExposure::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CPassAutoExposure::SetInputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputColorTexture != ptrColorTexture) {
		m_ptrInputColorTexture  = ptrColorTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_COLOR_TEXTURE_NAME, ptrColorTexture, pSamplerPoint);
	}
}

void CPassAutoExposure::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture) {
		m_ptrOutputColorTexture  = ptrColorTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentColor, ptrColorTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassAutoExposure::SetParamLower(float lower)
{
	m_lower = lower;
}

void CPassAutoExposure::SetParamUpper(float upper)
{
	m_upper = upper;
}

void CPassAutoExposure::SetParamLuminance(float luminance)
{
	m_luminance = luminance;
}

void CPassAutoExposure::SetParamLuminanceScaleRange(float minScaleLuminance, float maxScaleLuminance)
{
	m_minScaleLuminance = minScaleLuminance;
	m_maxScaleLuminance = maxScaleLuminance;
}

void CPassAutoExposure::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Update DescriptorSet
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetUniformBuffer(), m_pCamera->GetUniformBufferOffset(), m_pCamera->GetUniformBufferSize());
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferOffset(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferSize());
	m_ptrDescriptorSetPass->SetStorageBuffer(STORAGE_HISTOGRAM_DATA_NAME, m_pRenderSystem->GetHistogramStorage()->GetStorageBuffer(), m_pRenderSystem->GetHistogramStorage()->GetStorageBufferOffset(), m_pRenderSystem->GetHistogramStorage()->GetStorageBufferSize());

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassAutoExposure");
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

			m_pRenderQueue->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_AUTO_EXPOSURE_NAME, scissor, viewport, znear, zfar, 0xffffffff, false);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}

void CPassAutoExposure::RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer)
{
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.lower"), m_lower);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.upper"), m_upper);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.luminance"), m_luminance);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.minScaleLuminance"), m_minScaleLuminance);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.maxScaleLuminance"), m_maxScaleLuminance);
}
