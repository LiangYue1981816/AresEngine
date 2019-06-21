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

		m_ptrDescriptorSetShadowPass = GfxRenderer()->NewDescriptorSet(SHADOW_PASS_NAME, ptrDescriptorLayout);
		m_ptrDescriptorSetShadowPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pRenderSolution->GetShadowCameraUniformBuffer(), 0, m_pRenderSolution->GetShadowCameraUniformBuffer()->GetSize());
		m_ptrDescriptorSetShadowPass->Update();
	}

	// RenderPass and FrameBuffer
	{
		const int numAttachments = 1;
		const int numSubpasses = 1;

		m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Shadow"), numAttachments, numSubpasses);
		{
			m_ptrRenderPass->SetDepthStencilAttachment(0, m_pRenderSolution->GetShadowMapTexture(0)->GetFormat(), m_pRenderSolution->GetShadowMapTexture(0)->GetSamples(), true, true, 1.0f, 0);
			m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
		}
		m_ptrRenderPass->Create();

		for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
			m_ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(m_pRenderSolution->GetShadowMapTexture(indexFrame)->GetWidth(), m_pRenderSolution->GetShadowMapTexture(indexFrame)->GetHeight(), numAttachments);
			{
				m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, m_pRenderSolution->GetShadowMapTexture(indexFrame));
			}
			m_ptrFrameBuffer[indexFrame]->Create(m_ptrRenderPass);
		}
	}
}

CPassShadow::~CPassShadow(void)
{

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
	const glm::camera& mainCamera = m_pRenderSolution->GetMainCamera()->GetCamera();

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

		glm::sphere sphereFrustum = glm::sphere(minVertex, maxVertex);
	}
}

void CPassShadow::Render(int indexQueue)
{
	/*
	const int indexFrame = GfxRenderer()->GetSwapChain()->GetFrameIndex();

	const CGfxRenderPassPtr ptrRenderPass = m_ptrRenderPass;
	const CGfxFrameBufferPtr ptrFrameBuffer = m_ptrFrameBuffer[indexFrame];
	const CGfxRenderTexturePtr ptrShadowMapTexture = m_pRenderSolution->GetShadowMapTexture(indexFrame);

	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[indexFrame];
	{
		ptrMainCommandBuffer->Clearup();
		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrShadowMapTexture, GFX_IMAGE_LAYOUT_GENERAL);

			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
			{
//				m_pRenderSolution->GetMainQueue()->CmdDraw(indexQueue, ptrMainCommandBuffer, m_ptrDescriptorSetDefaultPass, DEFAULT_PASS_NAME, m_pRenderSolution->GetMainCamera()->GetScissor(), m_pRenderSolution->GetMainCamera()->GetViewport(), 0xffffffff);
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer);
	*/
}
