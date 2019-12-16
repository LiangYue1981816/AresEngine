#include "EngineHeader.h"
#include "RenderHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 1;
static CGfxRenderPassPtr ptrRenderPass;

void CPassShadow::Create(GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;

	ptrRenderPass = GfxRenderer()->NewRenderPass(PASS_SHADOW_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetDepthStencilAttachment(0, depthPixelFormat, 1, false, true, depth, stencil);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
	ptrRenderPass->Create();
}

void CPassShadow::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassShadow::CPassShadow(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
	, m_pShadowCamera{ nullptr }
	, m_pShadowRenderQueue{ nullptr }
	, m_factor(1.0f)
	, m_splitFactors{ 0.0f, exp(-3.0f), exp(-2.0f), exp(-1.0f), exp(-0.0f) }
{
	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		m_pShadowCamera[indexLevel] = new CGfxCamera;
		m_pShadowRenderQueue[indexLevel] = new CRenderQueue;
	}

	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetStorageBlockBinding(STORAGE_SCENE_NAME, STORAGE_SCENE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, UNIFORM_ENGINE_BIND);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, UNIFORM_CAMERA_BIND);
	ptrDescriptorLayout->Create();

	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		m_pShadowCameraUniform[indexLevel] = new CGfxUniformCamera;
		m_ptrDescriptorSetPass[indexLevel] = GfxRenderer()->NewDescriptorSet(HashValueFormat("%d_%x_%p", indexLevel, PASS_SHADOW_NAME, this), ptrDescriptorLayout);
		m_ptrDescriptorSetPass[indexLevel]->SetStorageBuffer(STORAGE_SCENE_NAME, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer(), 0, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer()->GetSize());
		m_ptrDescriptorSetPass[indexLevel]->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass[indexLevel]->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pShadowCameraUniform[indexLevel]->GetUniformBuffer(), 0, m_pShadowCameraUniform[indexLevel]->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass[indexLevel]->Update();
	}
}

CPassShadow::~CPassShadow(void)
{
	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		delete m_pShadowCamera[indexLevel];
		delete m_pShadowRenderQueue[indexLevel];
		delete m_pShadowCameraUniform[indexLevel];
	}
}

void CPassShadow::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CPassShadow::SetParamFactor(float factor)
{
	m_factor = factor;
}

void CPassShadow::SetParamSplitFactors(float f1, float f2, float f3, float f4)
{
	m_splitFactors[0] = 0.0f;
	m_splitFactors[1] = f1;
	m_splitFactors[2] = f2;
	m_splitFactors[3] = f3;
	m_splitFactors[4] = f4;
}

void CPassShadow::SetOutputTexture(CGfxRenderTexturePtr ptrDepthTexture)
{
	if (m_ptrOutputDepthTexture != ptrDepthTexture) {
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(ptrDepthTexture->GetWidth(), ptrDepthTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(0, ptrDepthTexture);
		m_ptrFrameBuffer->Create(ptrRenderPass);
		m_ptrOutputDepthTexture = ptrDepthTexture;
	}
}

void CPassShadow::Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrMainCommandBuffer)
{
	// Update
	const glm::camera mainCamera = m_pCamera->GetCamera()->GetCamera();
	const glm::vec4 mainLightDirection = m_pRenderSystem->GetEngineUniform()->GetParams().mainDirectLightDirection * glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);

	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		glm::vec3 minVertex = glm::vec3( FLT_MAX,  FLT_MAX,  FLT_MAX);
		glm::vec3 maxVertex = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (int indexFrustumLine = 0; indexFrustumLine < 4; indexFrustumLine++) {
			glm::vec4 vertex;
			glm::vec4 vertexBegin = mainCamera.getFrustumVertexInWorldSpace(indexFrustumLine);
			glm::vec4 vertexEnd = mainCamera.getFrustumVertexInWorldSpace(indexFrustumLine + 4);

			vertex = vertexBegin + (vertexEnd - vertexBegin) * m_factor * m_splitFactors[indexLevel];
			if (minVertex.x > vertex.x) minVertex.x = vertex.x;
			if (minVertex.y > vertex.y) minVertex.y = vertex.y;
			if (minVertex.z > vertex.z) minVertex.z = vertex.z;
			if (maxVertex.x < vertex.x) maxVertex.x = vertex.x;
			if (maxVertex.y < vertex.y) maxVertex.y = vertex.y;
			if (maxVertex.z < vertex.z) maxVertex.z = vertex.z;

			vertex = vertexBegin + (vertexEnd - vertexBegin) * m_factor * m_splitFactors[indexLevel + 1];
			if (minVertex.x > vertex.x) minVertex.x = vertex.x;
			if (minVertex.y > vertex.y) minVertex.y = vertex.y;
			if (minVertex.z > vertex.z) minVertex.z = vertex.z;
			if (maxVertex.x < vertex.x) maxVertex.x = vertex.x;
			if (maxVertex.y < vertex.y) maxVertex.y = vertex.y;
			if (maxVertex.z < vertex.z) maxVertex.z = vertex.z;
		}

		const float zFar = 500.0f;
		const float zNear = -500.0f;
		glm::sphere sphereFrustum = glm::sphere(minVertex, maxVertex);

		m_pShadowCamera[indexLevel]->SetOrtho(-sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
		m_pShadowCamera[indexLevel]->SetLookat(sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
		SceneManager()->UpdateCamera(taskGraph, m_pShadowCamera[indexLevel], m_pShadowRenderQueue[indexLevel], 0xffffffff);

		m_pShadowCameraUniform[indexLevel]->SetOrtho(-sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
		m_pShadowCameraUniform[indexLevel]->SetLookat(sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
		m_pShadowCameraUniform[indexLevel]->Apply();

		m_pRenderSystem->GetEngineUniform()->SetMainShadowLevelFactor(m_factor * m_splitFactors[1], m_factor * m_splitFactors[2], m_factor * m_splitFactors[3], m_factor * m_splitFactors[4]);
		m_pRenderSystem->GetEngineUniform()->SetMainShadowOrtho(indexLevel, -sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
		m_pRenderSystem->GetEngineUniform()->SetMainShadowLookat(indexLevel, sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
	}

	m_pCamera->GetCameraUniform()->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrOutputDepthTexture, GFX_IMAGE_LAYOUT_GENERAL);
	GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
	{
		const float w = m_ptrFrameBuffer->GetWidth();
		const float h = m_ptrFrameBuffer->GetHeight();

		const glm::vec4 area[4] = {
			glm::vec4(0.0f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
			glm::vec4(0.5f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
			glm::vec4(0.0f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
			glm::vec4(0.5f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
		};

		for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
//			m_pCamera->GetRenderQueue()->CmdDraw&taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass[indexLevel], PASS_SHADOW_NAME, area[indexLevel], area[indexLevel], 0xffffffff);
			m_pShadowRenderQueue[indexLevel]->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass[indexLevel], PASS_SHADOW_NAME, area[indexLevel], area[indexLevel], 0xffffffff, false);
		}
	}
	GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
	GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrOutputDepthTexture, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}
