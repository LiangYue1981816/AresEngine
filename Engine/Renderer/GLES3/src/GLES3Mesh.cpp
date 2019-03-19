#include "GLES3Renderer.h"


CGLES3Mesh::CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name)
	: CGfxMesh(name)
	, m_pManager(pManager)

	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
{

}

CGLES3Mesh::~CGLES3Mesh(void)
{
	Destroy();
}

void CGLES3Mesh::Release(void)
{
	m_pManager->Destroy(this);
}

CGfxMesh::Draw* CGLES3Mesh::GetDraw(int indexDraw)
{
	const auto &itDraw = m_draws.find(indexDraw);

	if (itDraw != m_draws.end()) {
		return &itDraw->second;
	}
	else {
		return nullptr;
	}
}

CGfxIndexBuffer* CGLES3Mesh::GetIndexBuffer(void)
{
	return m_pIndexBuffer;
}

CGfxVertexBuffer* CGLES3Mesh::GetVertexBuffer(void)
{
	return m_pVertexBuffer;
}

bool CGLES3Mesh::CreateDraw(int indexDraw, const glm::aabb &aabb, int baseVertex, int firstIndex, int indexCount)
{
	m_draws[indexDraw].aabb = aabb;
	m_draws[indexDraw].baseVertex = baseVertex;
	m_draws[indexDraw].firstIndex = firstIndex;
	m_draws[indexDraw].indexCount = indexCount;
	return true;
}

bool CGLES3Mesh::CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer)
{
	if (m_pIndexBuffer == nullptr) {
		m_pIndexBuffer = new CGLES3IndexBuffer(type, size, bDynamic);
		m_pIndexBuffer->BufferData(0, size, pBuffer);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic, const void *pBuffer)
{
	if (m_pVertexBuffer == nullptr) {
		m_pVertexBuffer = new CGLES3VertexBuffer(vertexFormat, vertexBinding, size, bDynamic);
		m_pVertexBuffer->BufferData(0, size, pBuffer);
		return true;
	}
	else {
		return false;
	}
}

void CGLES3Mesh::Destroy(void)
{
	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
	}

	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
	}

	m_draws.clear();
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
}
