#include "EngineHeader.h"


CPassShadow::CPassShadow(CRenderSolution* pRenderSolution)
	: m_pRenderSolution(pRenderSolution)
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
			m_ptrDescriptorSetShadowPass[indexLevel] = GfxRenderer()->NewDescriptorSet(SHADOW_PASS_NAME + indexLevel, ptrDescriptorLayout);
			m_ptrDescriptorSetShadowPass[indexLevel]->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pRenderSolution->GetShadowCameraUniform(indexLevel)->GetUniformBuffer(), 0, m_pRenderSolution->GetShadowCameraUniform(indexLevel)->GetUniformBuffer()->GetSize());
			m_ptrDescriptorSetShadowPass[indexLevel]->Update();
		}
	}

	// RenderPass and FrameBuffer
	{
		const int numAttachments = 1;
		const int numSubpasses = 1;

		m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Shadow"), numAttachments, numSubpasses);
		{
			m_ptrRenderPass->SetDepthStencilAttachment(0, m_pRenderSolution->GetShadowMapTexture()->GetFormat(), m_pRenderSolution->GetShadowMapTexture()->GetSamples(), false, true, 1.0f, 0);
			m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
		}
		m_ptrRenderPass->Create();

		m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_pRenderSolution->GetShadowMapTexture()->GetWidth(), m_pRenderSolution->GetShadowMapTexture()->GetHeight(), numAttachments);
		{
			m_ptrFrameBuffer->SetAttachmentTexture(0, m_pRenderSolution->GetShadowMapTexture());
		}
		m_ptrFrameBuffer->Create(m_ptrRenderPass);
	}
}

CPassShadow::~CPassShadow(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

const CGfxSemaphore* CPassShadow::GetSemaphore(void) const
{
	const int indexFrame = GfxRenderer()->GetSwapChain()->GetFrameIndex();
	return m_ptrMainCommandBuffer[indexFrame]->GetSemaphore();
}

void CPassShadow::SetSplitFactor(float f1, float f2, float f3)
{
	m_splitFactors[0] = 0.0f;
	m_splitFactors[1] = f1;
	m_splitFactors[2] = f2;
	m_splitFactors[3] = f3;
	m_splitFactors[4] = 1.0f;
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
	const int indexFrame = GfxRenderer()->GetSwapChain()->GetFrameIndex();

	const CGfxRenderPassPtr ptrRenderPass = m_ptrRenderPass;
	const CGfxFrameBufferPtr ptrFrameBuffer = m_ptrFrameBuffer;
	const CGfxRenderTexturePtr ptrShadowMapTexture = m_pRenderSolution->GetShadowMapTexture();

	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[indexFrame];
	{
		ptrMainCommandBuffer->Clearup();
		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrShadowMapTexture, GFX_IMAGE_LAYOUT_GENERAL);

			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
			{
				const float w = ptrShadowMapTexture->GetWidth();
				const float h = ptrShadowMapTexture->GetHeight();

				const glm::vec4 area[4] = {
					glm::vec4(0.0f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					glm::vec4(0.5f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					glm::vec4(0.0f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					glm::vec4(0.5f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
				};

				for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
					m_pRenderSolution->GetMainCameraQueue()->CmdDraw(indexQueue, ptrMainCommandBuffer, m_ptrDescriptorSetShadowPass[indexLevel], SHADOW_PASS_NAME, area[indexLevel], area[indexLevel], 0xffffffff);
				}
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
}
