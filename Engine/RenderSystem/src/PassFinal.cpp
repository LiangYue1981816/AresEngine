#include "EngineHeader.h"
#include "RenderHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassFinal::Create(GfxPixelFormat colorPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_FINAL_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->Create();
}

void CPassFinal::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassFinal::CPassFinal(CRenderSystem* pRenderSystem)
	: CPassBlit(PASS_FINAL_MATERIAL_NAME, pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_COLOR_TEXTURE_NAME, UNIFORM_COLOR_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_FINAL_NAME, this), ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
}

CPassFinal::~CPassFinal(void)
{

}

void CPassFinal::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassFinal::SetInputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputColorTexture != ptrColorTexture) {
		m_ptrInputColorTexture = ptrColorTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_COLOR_TEXTURE_NAME, ptrColorTexture, pSamplerPoint);
	}
}

void CPassFinal::SetOutputTexture(int indexFrame, CGfxRenderTexturePtr ptrColorTexture)
{
	if (m_ptrOutputColorTexture[indexFrame] != ptrColorTexture) {
		m_ptrOutputColorTexture[indexFrame] = ptrColorTexture;
		m_ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, ptrColorTexture);
		m_ptrFrameBuffer[indexFrame]->Create(ptrRenderPass);
	}
}

void CPassFinal::Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, int indexFrame, bool bPresent)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	const CGfxFrameBufferPtr ptrFrameBuffer = m_ptrFrameBuffer[indexFrame];
	const CGfxRenderTexturePtr ptrColorTexture = m_ptrOutputColorTexture[indexFrame];

	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassFinal");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, ptrColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, ptrFrameBuffer, ptrRenderPass);
		{
			const float w = ptrFrameBuffer->GetWidth();
			const float h = ptrFrameBuffer->GetHeight();
			const glm::vec4 scissor = glm::vec4(0.0, 0.0, w, h);
			const glm::vec4 viewport = glm::vec4(0.0, 0.0, w, h);

			m_pRenderQueue->CmdDraw(taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_FINAL_NAME, scissor, viewport, 0xffffffff, false);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, ptrColorTexture, bPresent ? GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL : GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
