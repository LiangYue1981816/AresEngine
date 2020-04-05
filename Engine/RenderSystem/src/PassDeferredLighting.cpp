#include "EngineHeader.h"
#include "RenderHeader.h"


static const int indexAttachmentColor = 0;
static const int indexAttachmentGBuffer0 = 1;
static const int indexAttachmentGBuffer1 = 2;
static const int indexAttachmentGBuffer2 = 3;
static const int indexAttachmentDepthStencil = 4;

static const int numSubpasses = 3;
static const int numAttachments = 5;
static CGfxRenderPassPtr ptrRenderPass;


void CPassDeferredLighting::Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_DEFERRED_LIGHTING_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentColor, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBuffer0, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBuffer1, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBuffer2, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(indexAttachmentDepthStencil, depthPixelFormat, 1, false, false, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBuffer0);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBuffer1);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBuffer2);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, indexAttachmentDepthStencil);
	ptrRenderPass->SetSubpassInputColorReference(1, indexAttachmentGBuffer0);
	ptrRenderPass->SetSubpassInputColorReference(1, indexAttachmentGBuffer1);
	ptrRenderPass->SetSubpassInputColorReference(1, indexAttachmentGBuffer2);
	ptrRenderPass->SetSubpassInputColorReference(1, indexAttachmentDepthStencil);
	ptrRenderPass->SetSubpassOutputColorReference(1, indexAttachmentColor);
	ptrRenderPass->SetSubpassOutputColorReference(2, indexAttachmentColor);
	ptrRenderPass->Create();
}

void CPassDeferredLighting::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassDeferredLighting::CPassDeferredLighting(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
{
	CGfxDescriptorLayoutPtr ptrDescriptorLayout0 = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout0->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout0->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout0->SetStorageBlockBinding(STORAGE_SCENE_DATA_NAME, STORAGE_SCENE_DATA_BIND);
	ptrDescriptorLayout0->Create();

	m_ptrDescriptorSetPass0 = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_DEFERRED_LIGHTING_GBUFFER_NAME, this), ptrDescriptorLayout0);
	m_ptrDescriptorSetPass0->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
	m_ptrDescriptorSetPass0->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer(), 0, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer()->GetSize());

	CGfxDescriptorLayoutPtr ptrDescriptorLayout1 = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout1->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout1->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout1->Create();

	m_ptrDescriptorSetPass1 = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_DEFERRED_LIGHTING_SHADING_NAME, this), ptrDescriptorLayout1);
	m_ptrDescriptorSetPass1->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());

	CGfxDescriptorLayoutPtr ptrDescriptorLayout2 = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout2->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout2->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout2->SetSampledImageBinding(UNIFORM_SSAO_TEXTURE_NAME, UNIFORM_SSAO_TEXTURE_BIND);
	ptrDescriptorLayout2->SetSampledImageBinding(UNIFORM_SHADOW_TEXTURE_NAME, UNIFORM_SHADOW_TEXTURE_BIND);
	ptrDescriptorLayout2->Create();

	m_ptrDescriptorSetPass2 = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_DEFERRED_LIGHTING_SHADOW_NAME, this), ptrDescriptorLayout2);
	m_ptrDescriptorSetPass2->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
}

CPassDeferredLighting::~CPassDeferredLighting(void)
{

}

void CPassDeferredLighting::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass0->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass1->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass2->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassDeferredLighting::SetInputTexture(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrSSAOTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	CGfxSampler* pSamplerLinear = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_LINEAR, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputShadowTexture != ptrShadowTexture) {
		m_ptrInputShadowTexture = ptrShadowTexture;
		m_ptrDescriptorSetPass2->SetRenderTexture(UNIFORM_SHADOW_TEXTURE_NAME, ptrShadowTexture, pSamplerPoint);
	}

	if (m_ptrInputSSAOTexture != ptrSSAOTexture) {
		m_ptrInputSSAOTexture = ptrSSAOTexture;
		m_ptrDescriptorSetPass2->SetRenderTexture(UNIFORM_SSAO_TEXTURE_NAME, ptrSSAOTexture, pSamplerLinear);
	}
}

void CPassDeferredLighting::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrGBuffer0Texture, CGfxRenderTexturePtr ptrGBuffer1Texture, CGfxRenderTexturePtr ptrGBuffer2Texture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture || m_ptrOutputGBuffer0Texture != ptrGBuffer0Texture ||m_ptrOutputGBuffer1Texture != ptrGBuffer1Texture || m_ptrOutputGBuffer2Texture != ptrGBuffer2Texture ||m_ptrOutputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrOutputColorTexture = ptrColorTexture;
		m_ptrOutputGBuffer0Texture = ptrGBuffer0Texture;
		m_ptrOutputGBuffer1Texture = ptrGBuffer1Texture;
		m_ptrOutputGBuffer2Texture = ptrGBuffer2Texture;
		m_ptrOutputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentColor, ptrColorTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBuffer0, ptrGBuffer0Texture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBuffer1, ptrGBuffer1Texture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBuffer2, ptrGBuffer2Texture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentDepthStencil, ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassDeferredLighting::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{

}
