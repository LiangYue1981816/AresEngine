#include "EngineHeader.h"


CPassBlit::CPassBlit(const char* szMaterialFileName)
	: m_pRenderQueue(nullptr)
{
	struct Vertex {
		float position[3];
		float texcoord[2];
	};

	const glm::aabb aabb;
	const int meshIndices[] = { 0, 1, 2, 2, 3, 0 };
	const Vertex meshVertices[] = { {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, -1.0f, 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f} };

	m_ptrMesh = GfxRenderer()->NewMesh(HashValue("Blit_Mesh"));
	m_ptrMesh->CreateDraw(0, aabb, 0, 0, 6);
	m_ptrMesh->CreateIndexBuffer(GFX_INDEX_UNSIGNED_INT, sizeof(meshIndices), false, (const void*)meshIndices);
	m_ptrMesh->CreateVertexBuffer(VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0, 0, sizeof(meshVertices), false, (const void*)meshVertices);

	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValue("Blit_MeshDraw"), m_ptrMesh, 0, INSTANCE_FORMAT_TRANSFORM);
	m_ptrMeshDraw->SetMask(0xffffffff);

	m_ptrMaterial = GfxRenderer()->NewMaterial(szMaterialFileName);

	const glm::mat4 matrix;
	m_pRenderQueue = new CGfxRenderQueue;
	m_pRenderQueue->Begin();
	m_pRenderQueue->Add(0, m_ptrMaterial, m_ptrMeshDraw, (const uint8_t*)&matrix, sizeof(matrix));
	m_pRenderQueue->End();
}

CPassBlit::~CPassBlit(void)
{
	delete m_pRenderQueue;
}

void CPassBlit::CmdBlit(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask)
{
	m_pRenderQueue->CmdDraw(taskGraph, ptrCommandBuffer, ptrDescriptorSetPass, matPassName, scissor, viewport, mask);
}
