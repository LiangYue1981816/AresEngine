#include "EngineHeader.h"


static CGfxMeshPtr ptrMesh;
static CGfxMeshDrawPtr ptrMeshDraw;
static CGfxRenderPassPtr ptrRenderPass;


CPassShadowMapBlur::CPassShadowMapBlur(CRenderSystem* pRenderSystem)
	: m_pRenderQueue(nullptr)
	, m_pRenderSystem(pRenderSystem)
{
	// RenderQueue
	{
		const glm::mat4 matrix;
		m_pRenderQueue = new CGfxRenderQueue;
		m_pRenderQueue->Begin();
		m_pRenderQueue->Add(0, GfxRenderer()->NewMaterial("ShadowBlur.material"), ptrMeshDraw, (const uint8_t*)&matrix, sizeof(matrix));
		m_pRenderQueue->End();
	}

	// CommandBuffer
	{
		m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
		m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
		m_ptrMainCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);
	}

	// DescriptorSet
	{
		CGfxDescriptorLayoutPtr ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, DESCRIPTOR_BIND_ENGINE);
		ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, DESCRIPTOR_BIND_CAMERA);
		ptrDescriptorLayout->SetSampledImageBinding(UNIFORM_SHADOWMAP_NAME, DESCRIPTOR_BIND_SHADOWMAP);
		ptrDescriptorLayout->Create();

		m_pCameraUniform = new CGfxUniformCamera;
		m_pCameraUniform->SetOrtho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
		m_pCameraUniform->SetLookat(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(SHADOWMAP_BLUR_PASS_NAME, ptrDescriptorLayout);
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCameraUniform->GetUniformBuffer(), 0, m_pCameraUniform->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->Update();
	}
}

CPassShadowMapBlur::~CPassShadowMapBlur(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();

	delete m_pRenderQueue;
	delete m_pCameraUniform;
}

void CPassShadowMapBlur::Create(GfxPixelFormat shadowPixelFormat)
{
	{
		const int numSubpasses = 1;
		const int numAttachments = 1;

		const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		ptrRenderPass = GfxRenderer()->NewRenderPass(SHADOWMAP_BLUR_PASS_NAME, numAttachments, numSubpasses);
		ptrRenderPass->SetColorAttachment(0, shadowPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
		ptrRenderPass->SetSubpassOutputColorReference(0, 0);
		ptrRenderPass->Create();
	}

	{
		struct Vertex {
			float position[3];
			float texcoord[2];
		};

		const glm::aabb aabb;
		const int meshIndices[] = { 0, 1, 2, 2, 3, 0 };
		const Vertex meshVertices[] = { {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, -1.0f, 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f} };

		ptrMesh = GfxRenderer()->NewMesh(HashValue("PassShadowBlur_Mesh"));
		ptrMesh->CreateDraw(0, aabb, 0, 0, 6);
		ptrMesh->CreateIndexBuffer(GFX_INDEX_UNSIGNED_INT, sizeof(meshIndices), false, (const void*)meshIndices);
		ptrMesh->CreateVertexBuffer(VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0, 0, sizeof(meshVertices), false, (const void*)meshVertices);

		ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValue("PassShadowBlur_MeshDraw"), ptrMesh, 0, INSTANCE_FORMAT_TRANSFORM);
		ptrMeshDraw->SetMask(0xffffffff);
	}
}

void CPassShadowMapBlur::Destroy(void)
{
	ptrMesh.Release();
	ptrMeshDraw.Release();
	ptrRenderPass.Release();
}

void CPassShadowMapBlur::CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowBlurTexture)
{
	const int numSubpasses = 1;
	const int numAttachments = 1;

	m_ptrShadowBlurTexture = ptrShadowBlurTexture;

	m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrShadowBlurTexture->GetWidth(), m_ptrShadowBlurTexture->GetHeight(), numAttachments);
	m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrShadowBlurTexture);
	m_ptrFrameBuffer->Create(ptrRenderPass);
}

void CPassShadowMapBlur::SetInputShadowTexture(CGfxRenderTexturePtr ptrShadowTexture)
{
	CGfxSampler* pSampler = GfxRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	m_ptrDescriptorSetPass->SetRenderTexture(UNIFORM_SHADOWMAP_NAME, ptrShadowTexture, pSampler);
	m_ptrDescriptorSetPass->Update();
}

const CGfxSemaphore* CPassShadowMapBlur::Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore)
{
	// Update
	m_pCameraUniform->Apply();
	m_pRenderSystem->GetEngineUniform()->Apply();

	// Render
	const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	{
		ptrMainCommandBuffer->Clearup();

		GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
		{
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrShadowBlurTexture, GFX_IMAGE_LAYOUT_GENERAL);
			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, m_ptrFrameBuffer, ptrRenderPass);
			{
				const float w = m_ptrShadowBlurTexture->GetWidth();
				const float h = m_ptrShadowBlurTexture->GetHeight();

				const glm::vec4 scissor[4] = {
					glm::vec4(0.0f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					glm::vec4(0.5f, 0.0f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					glm::vec4(0.0f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
					glm::vec4(0.5f, 0.5f, 0.5f, 0.5f) * glm::vec4(w, h, w, h),
				};

				const glm::vec4 viewport = glm::vec4(0.0, 0.0, w, h);

				for (int indexLevel = 0; indexLevel < 4; indexLevel++) {
					m_pRenderQueue->CmdDraw(taskGraph, ptrMainCommandBuffer, m_ptrDescriptorSetPass, SHADOWMAP_BLUR_PASS_NAME, scissor[indexLevel], viewport, 0xffffffff);
				}
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
			GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, m_ptrShadowBlurTexture, GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL);
		}
		GfxRenderer()->EndRecord(ptrMainCommandBuffer);
	}
	GfxRenderer()->Submit(ptrMainCommandBuffer, pWaitSemaphore);
	return ptrMainCommandBuffer->GetSemaphore();
}
