#include "RenderHeader.h"


CPassBlit::CPassBlit(const char* szMaterialFileName, CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
	, m_pRenderQueue(nullptr)
{
	struct Vertex {
		float position[3];
		float texcoord[2];
	};

	const glm::aabb aabb;
	const int meshIndices[] = { 0, 1, 2, 2, 3, 0 };
	const Vertex meshVertices[] = { {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, -1.0f, 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f} };

	m_ptrMesh = GfxRenderer()->NewMesh(HashValueFormat("%s.Mesh", szMaterialFileName));
	m_ptrMesh->CreateDraw(0, aabb, 0, 0, 6);
	m_ptrMesh->CreateIndexBuffer(GFX_INDEX_UINT32, sizeof(meshIndices), false, (const void*)meshIndices);
	m_ptrMesh->CreateVertexBuffer(VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0, 0, sizeof(meshVertices), false, (const void*)meshVertices);

	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%s.MeshDraw", szMaterialFileName), m_ptrMesh, 0);
	m_ptrMeshDraw->SetMask(0xffffffff);
	m_ptrMeshDraw->SetRenderCallback(CPassBlit::RenderCallback, this);

	m_ptrMaterial = GfxRenderer()->NewMaterial(szMaterialFileName, VERTEX_BINDING, INSTANCE_BINDING);

	m_pRenderQueue = new CRenderQueue;
	m_pRenderQueue->Begin(nullptr);
	m_pRenderQueue->Add(m_ptrMaterial, m_ptrMeshDraw, pRenderSystem->GetPostProcessInstnaceIndex());
	m_pRenderQueue->End();
}

CPassBlit::~CPassBlit(void)
{
	delete m_pRenderQueue;
}

void CPassBlit::RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer, void* param)
{
	if (CPassBlit* pPassBlit = (CPassBlit*)param) {
		pPassBlit->RenderCallback(ptrCommandBuffer);
	}
}
