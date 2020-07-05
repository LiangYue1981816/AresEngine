#include "EngineHeader.h"
#include "RenderHeader.h"


static const int indexAttachmentDepthStencil = 0;

static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassCopyDepthStencil::Create(GfxPixelFormat depthPixelFormat)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_COPY_DEPTH_STENCIL_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentDepthStencil, depthPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentDepthStencil);
	ptrRenderPass->Create();
}

void CPassCopyDepthStencil::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassCopyDepthStencil::CPassCopyDepthStencil(CRenderSystem* pRenderSystem)
	: CPassBlit("PassCopyDepthStencil.material", pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_DEPTH_TEXTURE_NAME, UNIFORM_DEPTH_TEXTURE_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_COPY_DEPTH_STENCIL_NAME, this), ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniformBuffer(), 0, m_pRenderSystem->GetEngineUniformBuffer()->GetSize());
}

CPassCopyDepthStencil::~CPassCopyDepthStencil(void)
{

}

void CPassCopyDepthStencil::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassCopyDepthStencil::SetInputTexture(CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrInputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_DEPTH_TEXTURE_NAME, ptrDepthStencilTexture, pSamplerPoint);
	}
}

void CPassCopyDepthStencil::SetOutputTexture(CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	if (m_ptrOutputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrOutputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrDepthStencilTexture->GetWidth(), ptrDepthStencilTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentDepthStencil, ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassCopyDepthStencil::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassCopyDepthStencil");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
		{
			const float w = m_ptrFrameBuffer->GetWidth();
			const float h = m_ptrFrameBuffer->GetHeight();
			const glm::vec4 scissor = glm::vec4(0.0, 0.0, w, h);
			const glm::vec4 viewport = glm::vec4(0.0, 0.0, w, h);
			const float znear = 0.0f;
			const float zfar = 1.0f;

			m_pRenderQueue->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass, PASS_COPY_DEPTH_STENCIL_NAME, scissor, viewport, znear, zfar, 0xffffffff, false);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
