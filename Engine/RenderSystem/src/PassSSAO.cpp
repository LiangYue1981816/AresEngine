#include "RenderHeader.h"


static const int indexAttachmentColor = 0;

static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassSSAO::Create(GfxPixelFormat colorPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_SSAO_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentColor, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentColor);
	ptrRenderPass->Create();
}

void CPassSSAO::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassSSAO::CPassSSAO(CRenderSystem* pRenderSystem)
	: CPassBlit("PassSSAO.material", pRenderSystem)
	, m_samples(16)
	, m_minSampleRadius(0.02f)
	, m_maxSampleRadius(1.25f)
	, m_minDepthRange(0.0f)
	, m_maxDepthRange(0.2f)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_DEPTH_TEXTURE_NAME, UNIFORM_DEPTH_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_SSAO_NAME, this), ptrDescriptorLayout);
}

CPassSSAO::~CPassSSAO(void)
{

}

void CPassSSAO::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CPassSSAO::SetInputTexture(CGfxRenderTexturePtr ptrDepthTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputDepthTexture != ptrDepthTexture) {
		m_ptrInputDepthTexture  = ptrDepthTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_DEPTH_TEXTURE_NAME, ptrDepthTexture, pSamplerPoint);
	}
}

void CPassSSAO::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture) {
		m_ptrOutputColorTexture  = ptrColorTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentColor, ptrColorTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassSSAO::SetParamSamples(int samples)
{
	m_samples = samples;
}

void CPassSSAO::SetParamMinSampleRadius(float minRadius)
{
	m_minSampleRadius = minRadius;
}

void CPassSSAO::SetParamMaxSampleRadius(float maxRadius)
{
	m_maxSampleRadius = maxRadius;
}

void CPassSSAO::SetParamMinDepthRange(float minDepth)
{
	m_minDepthRange = minDepth;
}

void CPassSSAO::SetParamMaxDepthRange(float maxDepth)
{
	m_maxDepthRange = maxDepth;
}

void CPassSSAO::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Update DescriptorSet
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetUniformBuffer(), m_pCamera->GetUniformBufferOffset(), m_pCamera->GetUniformBufferSize());
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferOffset(), m_pRenderSystem->GetEngineUniform()->GetUniformBufferSize());

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassSSAO");
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

			m_pRenderQueue->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_SSAO_NAME, scissor, viewport, znear, zfar, 0xffffffff, false);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}

void CPassSSAO::RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer)
{
	GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.samples"), m_samples);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.minSampleRadius"), m_minSampleRadius);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.maxSampleRadius"), m_maxSampleRadius);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.minDepthRange"), m_minDepthRange);
	GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.maxDepthRange"), m_maxDepthRange);
}
