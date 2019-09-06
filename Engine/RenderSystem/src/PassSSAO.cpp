#include "EngineHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassSSAO::Create(GfxPixelFormat colorPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_SSAO_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->Create();
}

void CPassSSAO::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassSSAO::CPassSSAO(CRenderSystem* pRenderSystem)
	: CPassBlit("SSAO.material", pRenderSystem->GetEngineUniform())
{
	// CommandBuffer
	m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);

	// DescriptorLayout and DescriptorSet
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_DEPTH_TEXTURE_NAME, UNIFORM_DEPTH_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(PASS_SSAO_NAME, ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pEngineUniform->GetUniformBuffer(), 0, m_pEngineUniform->GetUniformBuffer()->GetSize());
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCameraUniform->GetUniformBuffer(), 0, m_pCameraUniform->GetUniformBuffer()->GetSize());
}

CPassSSAO::~CPassSSAO(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

void CPassSSAO::SetInputDepthTexture(CGfxRenderTexturePtr ptrDepthTexture)
{
	CGfxSampler* pSampler = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_DEPTH_TEXTURE_NAME, ptrDepthTexture, pSampler);
}

void CPassSSAO::SetOutputTexture(CGfxRenderTexturePtr ptrColorGradingTexture)
{
	m_ptrColorTexture = ptrColorGradingTexture;

	m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrColorTexture->GetWidth(), m_ptrColorTexture->GetHeight(), numAttachments);
	m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrColorTexture);
	m_ptrFrameBuffer->Create(ptrRenderPass);
}

const CGfxSemaphore* CPassSSAO::Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore)
{
	// Update
	m_pCameraUniform->Apply();
	m_pEngineUniform->Apply();
	m_ptrDescriptorSetPass->Update();

	// Render
	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	{
		ptrMainCommandBuffer->Clearup();

		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
			{
				const float w = m_ptrColorTexture->GetWidth();
				const float h = m_ptrColorTexture->GetHeight();
				const glm::vec4 scissor = glm::vec4(0.0, 0.0, w, h);
				const glm::vec4 viewport = glm::vec4(0.0, 0.0, w, h);

				m_pRenderQueue->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass, PASS_SSAO_NAME, scissor, viewport, 0xffffffff);
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
	return ptrMainCommandBuffer->GetSemaphore();
}
