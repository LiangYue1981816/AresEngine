#include "EngineHeader.h"
#include "RenderHeader.h"


static const int indexAttachmentGBufferA = 0;
static const int indexAttachmentGBufferB = 1;
static const int indexAttachmentGBufferC = 2;
static const int indexAttachmentDepthStencil = 3;

static const int numSubpasses = 1;
static const int numAttachments = 4;
static CGfxRenderPassPtr ptrRenderPass;


void CPassGBuffer::Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_GBUFFER_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBufferA, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBufferB, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBufferC, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(indexAttachmentDepthStencil, depthPixelFormat, 1, false, false, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBufferA);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBufferB);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBufferC);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, indexAttachmentDepthStencil);
	ptrRenderPass->Create();
}

void CPassGBuffer::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassGBuffer::CPassGBuffer(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_DATA_NAME, STORAGE_SCENE_DATA_BIND);
	ptrDescriptorLayout->Create();

	m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_GBUFFER_NAME, this), ptrDescriptorLayout);
	m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
	m_ptrDescriptorSetPass->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer(), 0, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer()->GetSize());
}

CPassGBuffer::~CPassGBuffer(void)
{

}

void CPassGBuffer::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassGBuffer::SetOutputTexture(CGfxRenderTexturePtr ptrGBufferATexture, CGfxRenderTexturePtr ptrGBufferBTexture, CGfxRenderTexturePtr ptrGBufferCTexture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	if (m_ptrOutputGBufferATexture != ptrGBufferATexture ||m_ptrOutputGBufferBTexture != ptrGBufferBTexture || m_ptrOutputGBufferCTexture != ptrGBufferCTexture ||m_ptrOutputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrOutputGBufferATexture = ptrGBufferATexture;
		m_ptrOutputGBufferBTexture = ptrGBufferBTexture;
		m_ptrOutputGBufferCTexture = ptrGBufferCTexture;
		m_ptrOutputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBufferA, ptrGBufferATexture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBufferB, ptrGBufferBTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBufferC, ptrGBufferCTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentDepthStencil, ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassGBuffer::Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{

}
