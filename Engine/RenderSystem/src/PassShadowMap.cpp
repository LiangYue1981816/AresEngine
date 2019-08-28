#include "EngineHeader.h"


static const int numSubpasses = 1;
static const int numAttachments = 2;
static CGfxRenderPassPtr ptrRenderPass;

void CPassShadowMap::Create(GfxPixelFormat shadowPixelFormat, GfxPixelFormat depthPixelFormat)
{
	const int stencil = 0;
	const float depth = 1.0f;
	const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ptrRenderPass = GfxRenderer()->NewRenderPass(SHADOWMAP_PASS_NAME, numAttachments, numSubpasses);
	ptrRenderPass->SetColorAttachment(0, shadowPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
	ptrRenderPass->SetDepthStencilAttachment(1, depthPixelFormat, 1, true, true, depth, stencil);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void CPassShadowMap::Destroy(void)
{
	ptrRenderPass.Release();
}


CPassShadowMap::CPassShadowMap(CRenderSystem* pRenderSystem)
	: m_pCamera(nullptr)
	, m_pRenderSystem(pRenderSystem)

	, m_pShadowCamera{ nullptr }
	, m_pShadowRenderQueue{ nullptr }
	, m_splitFactors{ 0.0f, exp(-3.0f), exp(-2.0f), exp(-1.0f), exp(-0.0f) }
{
	// Camera
	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		m_pShadowCamera[indexLevel] = new CGfxCamera;
		m_pShadowRenderQueue[indexLevel] = new CGfxRenderQueue;
	}

	// CommandBuffer
	m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);

	// DescriptorLayout and DescriptorSet
	CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, DESCRIPTOR_BIND_ENGINE);
	ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, DESCRIPTOR_BIND_CAMERA);
	ptrDescriptorLayout->Create();

	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		m_pShadowCameraUniform[indexLevel] = new CGfxUniformCamera;
		m_ptrDescriptorSetPass[indexLevel] = GfxRenderer()->NewDescriptorSet(SHADOWMAP_PASS_NAME + indexLevel, ptrDescriptorLayout);
		m_ptrDescriptorSetPass[indexLevel]->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass[indexLevel]->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pShadowCameraUniform[indexLevel]->GetUniformBuffer(), 0, m_pShadowCameraUniform[indexLevel]->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass[indexLevel]->Update();
	}
}

CPassShadowMap::~CPassShadowMap(void)
{
	for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
		delete m_pShadowCamera[indexLevel];
		delete m_pShadowRenderQueue[indexLevel];
		delete m_pShadowCameraUniform[indexLevel];
	}
}

void CPassShadowMap::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CPassShadowMap::SetFrameBuffer(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrDepthStencilTexture)
{
	m_ptrShadowTexture = ptrShadowTexture;
	m_ptrDepthStencilTexture = ptrDepthStencilTexture;

	m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrShadowTexture->GetWidth(), m_ptrShadowTexture->GetHeight(), numAttachments);
	m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrShadowTexture);
	m_ptrFrameBuffer->SetAttachmentTexture(1, m_ptrDepthStencilTexture);
	m_ptrFrameBuffer->Create(ptrRenderPass);
}

const CGfxSemaphore* CPassShadowMap::Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore)
{
	if (m_pCamera) {
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

				vertex = vertexBegin + (vertexEnd - vertexBegin) * m_splitFactors[indexLevel];
				if (minVertex.x > vertex.x) minVertex.x = vertex.x;
				if (minVertex.y > vertex.y) minVertex.y = vertex.y;
				if (minVertex.z > vertex.z) minVertex.z = vertex.z;
				if (maxVertex.x < vertex.x) maxVertex.x = vertex.x;
				if (maxVertex.y < vertex.y) maxVertex.y = vertex.y;
				if (maxVertex.z < vertex.z) maxVertex.z = vertex.z;

				vertex = vertexBegin + (vertexEnd - vertexBegin) * m_splitFactors[indexLevel + 1];
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
			sphereFrustum.radius *= 0.75f; // ???

			m_pShadowCamera[indexLevel]->SetOrtho(-sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
			m_pShadowCamera[indexLevel]->SetLookat(sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
			SceneManager()->UpdateCamera(taskGraph, m_pShadowCamera[indexLevel], m_pShadowRenderQueue[indexLevel], 0xffffffff);

			m_pShadowCameraUniform[indexLevel]->SetOrtho(-sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
			m_pShadowCameraUniform[indexLevel]->SetLookat(sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
			m_pShadowCameraUniform[indexLevel]->Apply();

			m_pRenderSystem->GetEngineUniform()->SetMainShadowLevelFactor(m_splitFactors[1], m_splitFactors[2], m_splitFactors[3], m_splitFactors[4]);
			m_pRenderSystem->GetEngineUniform()->SetMainShadowOrtho(indexLevel, -sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
			m_pRenderSystem->GetEngineUniform()->SetMainShadowLookat(indexLevel, sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
		}

		m_pCamera->GetCameraUniform()->Apply();
		m_pRenderSystem->GetEngineUniform()->Apply();

		// Render
		const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
		{
			ptrMainCommandBuffer->Clearup();

			GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
			{
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrShadowTexture, GFX_IMAGE_LAYOUT_GENERAL);
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
				GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
				{
					const float w = m_ptrShadowTexture->GetWidth();
					const float h = m_ptrShadowTexture->GetHeight();

					const glm::vec4 area[4] = {
						glm::vec4(0.0f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
						glm::vec4(0.5f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
						glm::vec4(0.0f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
						glm::vec4(0.5f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					};

					for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
//						m_pCamera->GetRenderQueue()->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass[indexLevel], SHADOWMAP_PASS_NAME, area[indexLevel], area[indexLevel], 0xffffffff);
						m_pShadowRenderQueue[indexLevel]->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass[indexLevel], SHADOWMAP_PASS_NAME, area[indexLevel], area[indexLevel], 0xffffffff);
					}
				}
				GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
				GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrShadowTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
			}
			GfxRenderer()->EndRecord(ptrMainCommandBuffer);
		}
		GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
		return ptrMainCommandBuffer->GetSemaphore();
	}
	else {
		return nullptr;
	}
}
