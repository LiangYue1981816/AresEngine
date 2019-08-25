#include "EngineHeader.h"


static CGfxMeshPtr ptrMesh;
static CGfxMeshDrawPtr ptrMeshDraw;
static CGfxRenderPassPtr ptrRenderPass;


CPassShadowBlur::CPassShadowBlur(CRenderSystem* pRenderSystem)
	: m_pCamera(nullptr)
	, m_pRenderQueue(nullptr)
	, m_pRenderSystem(pRenderSystem)
{
	// Camera
	{
		m_pCamera = new CGfxCamera;
		m_pRenderQueue = new CGfxRenderQueue;
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
		ptrDescriptorLayout->Create();

		m_pCameraUniform = new CGfxUniformCamera;
		m_ptrDescriptorSetPass = GfxRenderer()->NewDescriptorSet(SHADOW_BLUR_PASS_NAME, ptrDescriptorLayout);
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer(), 0, m_pRenderSystem->GetEngineUniform()->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCameraUniform->GetUniformBuffer(), 0, m_pCameraUniform->GetUniformBuffer()->GetSize());
		m_ptrDescriptorSetPass->Update();
	}
}

CPassShadowBlur::~CPassShadowBlur(void)
{
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();

	delete m_pCamera;
	delete m_pRenderQueue;
	delete m_pCameraUniform;
}

void CPassShadowBlur::Create(GfxPixelFormat shadowPixelFormat)
{
	{
		const int numSubpasses = 1;
		const int numAttachments = 2;

		const int stencil = 0;
		const float depth = 1.0f;
		const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Shadow"), numAttachments, numSubpasses);
		ptrRenderPass->SetColorAttachment(0, shadowPixelFormat, 1, false, false, color[0], color[1], color[2], color[3]);
		ptrRenderPass->SetColorAttachment(1, shadowPixelFormat, 1, false, true, color[0], color[1], color[2], color[3]);
		ptrRenderPass->SetSubpassInputColorReference(0, 0);
		ptrRenderPass->SetSubpassOutputColorReference(0, 1);
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

		ptrMesh = GfxRenderer()->NewMesh(HashValue("PassShadow_Mesh"));
		ptrMesh->CreateDraw(0, aabb, 0, 0, 6);
		ptrMesh->CreateIndexBuffer(GFX_INDEX_UNSIGNED_INT, sizeof(meshIndices), false, (const void*)meshIndices);
		ptrMesh->CreateVertexBuffer(VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0, 0, sizeof(meshVertices), false, (const void*)meshVertices);

		ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValue("PassShadow_MeshDraw"), ptrMesh, 0, INSTANCE_FORMAT_TRANSFORM);
	}
}

void CPassShadowBlur::Destroy(void)
{
	ptrMesh.Release();
	ptrMeshDraw.Release();
	ptrRenderPass.Release();
}

void CPassShadowBlur::CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrShadowBlurTexture)
{
	const int numSubpasses = 1;
	const int numAttachments = 2;

	m_ptrShadowTexture = ptrShadowTexture;
	m_ptrShadowBlurTexture = ptrShadowBlurTexture;

	m_ptrFrameBuffer = GfxRenderer()->NewFrameBuffer(m_ptrShadowBlurTexture->GetWidth(), m_ptrShadowBlurTexture->GetHeight(), numAttachments);
	m_ptrFrameBuffer->SetAttachmentTexture(0, m_ptrShadowTexture);
	m_ptrFrameBuffer->SetAttachmentTexture(1, m_ptrShadowBlurTexture);
	m_ptrFrameBuffer->Create(ptrRenderPass);
}

const CGfxSemaphore* CPassShadowBlur::Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore)
{
	return nullptr;
}
