#include "EngineHeader.h"


CPassShadow::CPassShadow(CCamera* pCamera, CRenderSolution* pRenderSolution)
	: m_pCamera(pCamera)
	, m_pRenderSolution(pRenderSolution)
	, m_splitFactors{ 0.0f, 0.1f, 0.2f, 0.5f, 1.0f }
{
	// CommandBuffer
	{
		m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
		m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
		m_ptrMainCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);
	}

	// DescriptorSet
	{
		CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, DESCRIPTOR_BIND_CAMERA);
		ptrDescriptorLayout->Create();

		for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
			m_ptrDescriptorSetPass[indexLevel] = GfxRenderer()->NewDescriptorSet(SHADOW_PASS_NAME + indexLevel, ptrDescriptorLayout);
			m_ptrDescriptorSetPass[indexLevel]->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pRenderSolution->GetShadowCameraUniform(indexLevel)->GetUniformBuffer(), 0, m_pRenderSolution->GetShadowCameraUniform(indexLevel)->GetUniformBuffer()->GetSize());
			m_ptrDescriptorSetPass[indexLevel]->Update();
		}
	}
}

CPassShadow::~CPassShadow(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

void CPassShadow::CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowTexture)
{
	const int numSubpasses = 1;
	const int numAttachments = 1;

	m_ptrShadowTexture = ptrShadowTexture;

	// RenderPass
	{
		const int stencil = 0;
		const float depth = 1.0f;

		m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Shadow"), numAttachments, numSubpasses);
		m_ptrRenderPass->SetDepthStencilAttachment(0, m_ptrShadowTexture->GetFormat(), m_ptrShadowTexture->GetSamples(), false, true, depth, stencil);
		m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
		m_ptrRenderPass->Create();
	}

	// FrameBuffer
	{
		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrShadowTexture->GetWidth(), m_ptrShadowTexture->GetHeight(), numAttachments);
		m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrShadowTexture);
		m_ptrFrameBuffer->Create(m_ptrRenderPass);
	}
}

void CPassShadow::Update(void)
{
	const glm::camera mainCamera = m_pRenderSolution->GetMainCamera()->GetCamera();
	const glm::vec4 mainLightDirection = m_pRenderSolution->GetEngineUniform()->GetParams().mainDirectLightDirection * glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);

	for (int indexFrustum = 0; indexFrustum < 4; indexFrustum++) {
		glm::vec3 minVertex = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		glm::vec3 maxVertex = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (int indexFrustumLine = 0; indexFrustumLine < 4; indexFrustumLine++) {
			glm::vec4 vertex;
			glm::vec4 vertexBegin = mainCamera.getFrustumVertexInWorldSpace(indexFrustumLine);
			glm::vec4 vertexEnd = mainCamera.getFrustumVertexInWorldSpace(indexFrustumLine + 4);

			vertex = vertexBegin + (vertexEnd - vertexBegin) * m_splitFactors[indexFrustum];
			if (minVertex.x > vertex.x) minVertex.x = vertex.x;
			if (minVertex.y > vertex.y) minVertex.y = vertex.y;
			if (minVertex.z > vertex.z) minVertex.z = vertex.z;
			if (maxVertex.x < vertex.x) maxVertex.x = vertex.x;
			if (maxVertex.y < vertex.y) maxVertex.y = vertex.y;
			if (maxVertex.z < vertex.z) maxVertex.z = vertex.z;

			vertex = vertexBegin + (vertexEnd - vertexBegin) * m_splitFactors[indexFrustum + 1];
			if (minVertex.x > vertex.x) minVertex.x = vertex.x;
			if (minVertex.y > vertex.y) minVertex.y = vertex.y;
			if (minVertex.z > vertex.z) minVertex.z = vertex.z;
			if (maxVertex.x < vertex.x) maxVertex.x = vertex.x;
			if (maxVertex.y < vertex.y) maxVertex.y = vertex.y;
			if (maxVertex.z < vertex.z) maxVertex.z = vertex.z;
		}

		float zNear = -100.0f;
		float zFar = 100.0f;
		float resolution = m_pRenderSolution->GetShadowMapTexture()->GetWidth();
		glm::sphere sphereFrustum = glm::sphere(minVertex, maxVertex);

		m_pRenderSolution->GetShadowCameraUniform(indexFrustum)->SetOrtho(-sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
		m_pRenderSolution->GetShadowCameraUniform(indexFrustum)->SetLookat(sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);

		m_pRenderSolution->GetEngineUniform()->SetShadowOrtho(indexFrustum, -sphereFrustum.radius, sphereFrustum.radius, -sphereFrustum.radius, sphereFrustum.radius, zNear, zFar);
		m_pRenderSolution->GetEngineUniform()->SetShadowLookat(indexFrustum, sphereFrustum.center.x, sphereFrustum.center.y, sphereFrustum.center.z, sphereFrustum.center.x + mainLightDirection.x, sphereFrustum.center.y + mainLightDirection.y, sphereFrustum.center.z + mainLightDirection.z, 0.0f, 1.0f, 0.0f);
		m_pRenderSolution->GetEngineUniform()->SetShadowRange(indexFrustum, zFar - zNear);
		m_pRenderSolution->GetEngineUniform()->SetShadowResolution(indexFrustum, resolution);
	}
}

void CPassShadow::Render(int indexQueue, const CGfxSemaphore* pWaitSemaphore)
{
	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	{
		ptrMainCommandBuffer->Clearup();

		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrShadowTexture, GFX_IMAGE_LAYOUT_GENERAL);

			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass);
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
					m_pCamera->GetRenderQueue()->CmdDraw(indexQueue, ptrMainCommandBuffer, m_ptrDescriptorSetPass[indexLevel], SHADOW_PASS_NAME, area[indexLevel], area[indexLevel], 0xffffffff);
				}
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
}

const CGfxSemaphore* CPassShadow::GetSemaphore(void) const
{
	return m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()]->GetSemaphore();
}
