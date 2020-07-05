#include "EngineHeader.h"
#include "RenderHeader.h"


////////////////////////////////////////////////////////////////////////////////////
// SubPass0 AmbientLighting DirectionLighting EnvLighting GBuffer
//    Color = ao * ssao * (ambientLighting + directLighting * shadow + envLighting)
//    GBuffer0 = Albedo
//    GBuffer1 = Normal
//    GBuffer2 = Roughness Metallic Specular (AO * SSAO)
// SubPass1 PointLighting
//    Color += ao * ssao * pointLighting
//==================================================================================

static const int indexAttachmentColor = 0;
static const int indexAttachmentGBuffer0 = 1;
static const int indexAttachmentGBuffer1 = 2;
static const int indexAttachmentDepthStencil = 3;

static const int numSubpasses = 2;
static const int numAttachments = 4;
static CGfxRenderPassPtr ptrRenderPass;


void CPassTileDeferredShading::Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.1f, 0.1f, 0.1f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_TILE_DEFERRED_SHADING_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(indexAttachmentColor, colorPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBuffer0, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetColorAttachment(indexAttachmentGBuffer1, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(indexAttachmentDepthStencil, depthPixelFormat, 1, false, false, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentColor);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBuffer0);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexAttachmentGBuffer1);
	ptrRenderPass->SetSubpassDepthStencilReference(0, indexAttachmentDepthStencil);
	ptrRenderPass->SetSubpassInputColorReference(1, indexAttachmentGBuffer0);
	ptrRenderPass->SetSubpassInputColorReference(1, indexAttachmentGBuffer1);
	ptrRenderPass->SetSubpassOutputColorReference(1, indexAttachmentColor);
	ptrRenderPass->Create();
}

void CPassTileDeferredShading::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassTileDeferredShading::CPassTileDeferredShading(CRenderSystem* pRenderSystem)
	: CPassBlit("PassTileDeferredShading.material", pRenderSystem)
{
	{
		CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
		ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_DATA_NAME, STORAGE_SCENE_DATA_BIND);
		ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_SSAO_TEXTURE_NAME, UNIFORM_SSAO_TEXTURE_BIND);
		ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_SHADOW_TEXTURE_NAME, UNIFORM_SHADOW_TEXTURE_BIND);
		ptrDescriptorLayout->Create();

		m_ptrDescriptorSetPass_Subpass0 = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_TILE_DEFERRED_SHADING_GBUFFER_NAME, this), ptrDescriptorLayout);
		m_ptrDescriptorSetPass_Subpass0->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass_Subpass0->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceDataBuffer(), 0, m_pRenderSystem->GetInstanceDataBuffer()->GetSize());
	}

	{
		CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
		ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_DEPTH_TEXTURE_NAME, UNIFORM_DEPTH_TEXTURE_BIND);
		ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_DATA_NAME, STORAGE_SCENE_DATA_BIND);
		ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_CLUSTER_DATA_NAME, STORAGE_CLUSTER_DATA_BIND);
		ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_CULL_LIGHT_LIST_DATA_NAME, STORAGE_CULL_LIGHT_LIST_DATA_BIND);
		ptrDescriptorLayout->Create();

		m_ptrDescriptorSetPass_Subpass1 = GfxRenderer()->NewDescriptorSet(HashValueFormat("%x_%p", PASS_TILE_DEFERRED_SHADING_LIGHTING_NAME, this), ptrDescriptorLayout);
		m_ptrDescriptorSetPass_Subpass1->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass_Subpass1->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceDataBuffer(), 0, m_pRenderSystem->GetInstanceDataBuffer()->GetSize());
		m_ptrDescriptorSetPass_Subpass1->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_pRenderSystem->GetCluster()->GetClusterBuffer(), 0, m_pRenderSystem->GetCluster()->GetClusterBuffer()->GetSize());
		m_ptrDescriptorSetPass_Subpass1->SetStorageBuffer(STORAGE_CULL_LIGHT_LIST_DATA_NAME, m_pRenderSystem->GetCluster()->GetCullLightListBuffer(), 0, m_pRenderSystem->GetCluster()->GetCullLightListBuffer()->GetSize());
	}
}

CPassTileDeferredShading::~CPassTileDeferredShading(void)
{

}

void CPassTileDeferredShading::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSetPass_Subpass0->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass_Subpass1->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CPassTileDeferredShading::SetInputTexture(CGfxRenderTexturePtr ptrDepthTexture, CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrSSAOTexture)
{
	CGfxSampler* pSamplerPoint = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	CGfxSampler* pSamplerLinear = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_LINEAR, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	if (m_ptrInputShadowTexture != ptrShadowTexture) {
		m_ptrInputShadowTexture = ptrShadowTexture;
		m_ptrDescriptorSetPass_Subpass0->SetRenderTexture(UNIFORM_SHADOW_TEXTURE_NAME, ptrShadowTexture, pSamplerPoint);
	}

	if (m_ptrInputSSAOTexture != ptrSSAOTexture) {
		m_ptrInputSSAOTexture = ptrSSAOTexture;
		m_ptrDescriptorSetPass_Subpass0->SetRenderTexture(UNIFORM_SSAO_TEXTURE_NAME, ptrSSAOTexture, pSamplerLinear);
	}

	if (m_ptrInputDepthTexture != ptrDepthTexture) {
		m_ptrInputDepthTexture = ptrDepthTexture;
		m_ptrDescriptorSetPass_Subpass1->SetRenderTexture(UNIFORM_DEPTH_TEXTURE_NAME, ptrDepthTexture, pSamplerPoint);
	}
}

void CPassTileDeferredShading::SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrGBuffer0Texture, CGfxRenderTexturePtr ptrGBuffer1Texture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	if (m_ptrOutputColorTexture != ptrColorTexture || m_ptrOutputGBuffer0Texture != ptrGBuffer0Texture || m_ptrOutputGBuffer1Texture != ptrGBuffer1Texture || m_ptrOutputDepthStencilTexture != ptrDepthStencilTexture) {
		m_ptrOutputColorTexture = ptrColorTexture;
		m_ptrOutputGBuffer0Texture = ptrGBuffer0Texture;
		m_ptrOutputGBuffer1Texture = ptrGBuffer1Texture;
		m_ptrOutputDepthStencilTexture = ptrDepthStencilTexture;
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentColor, ptrColorTexture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBuffer0, ptrGBuffer0Texture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentGBuffer1, ptrGBuffer1Texture);
		m_ptrFrameBuffer->SetAttachmentTexture(indexAttachmentDepthStencil, ptrDepthStencilTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
	}
}

void CPassTileDeferredShading::Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "PassTileDeferredShading");
	{
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputGBuffer0Texture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputGBuffer1Texture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
		GfxRenderer()->CmdBeginRenderPass(ptrCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
		{
			const float znear = 0.0f;
			const float zfar = 1.0f;

			m_pCamera->GetRenderQueue()->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass_Subpass0, PASS_TILE_DEFERRED_SHADING_GBUFFER_NAME, m_pCamera->GetScissor(), m_pCamera->GetViewport(), m_pCamera->GetZNear(), m_pCamera->GetZFar(), 0xffffffff, false);
			ptrCommandBuffer->CmdNextSubpass();
			m_pRenderQueue->CmdDraw(taskPool, taskGraph, ptrCommandBuffer, m_ptrDescriptorSetPass_Subpass1, PASS_TILE_DEFERRED_SHADING_LIGHTING_NAME, m_pCamera->GetScissor(), m_pCamera->GetViewport(), znear, zfar, 0xffffffff, true);
		}
		GfxRenderer()->CmdEndRenderPass(ptrCommandBuffer);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputColorTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputGBuffer0Texture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputGBuffer1Texture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		GfxRenderer()->CmdSetImageLayout(ptrCommandBuffer, m_ptrOutputDepthStencilTexture, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}

void CPassTileDeferredShading::RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer)
{
	GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numDepthTiles"), CLUSTER_DEPTH_TILE_COUNT);
	GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numWidthTiles"), CLUSTER_WIDTH_TILE_COUNT);
	GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numHeightTiles"), CLUSTER_HEIGHT_TILE_COUNT);
}
