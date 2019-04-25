#include "GLES3Renderer.h"


CGLES3Mesh::CGLES3Mesh(CGLES3MeshManager* pManager, uint32_t name)
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

CGfxMesh::Draw* CGLES3Mesh::GetDraw(uint32_t name)
{
	const auto& itDraw = m_draws.find(name);

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

bool CGLES3Mesh::CreateDraw(uint32_t name, const glm::aabb& aabb, int baseVertex, int firstIndex, int indexCount)
{
	m_draws[name].aabb = aabb;
	m_draws[name].baseVertex = baseVertex;
	m_draws[name].firstIndex = firstIndex;
	m_draws[name].indexCount = indexCount;
	return true;
}

bool CGLES3Mesh::CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void* data)
{
	if (m_pIndexBuffer == nullptr) {
		m_pIndexBuffer = new CGLES3IndexBuffer(type, size, bDynamic);
		return m_pIndexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data)
{
	if (m_pVertexBuffer == nullptr) {
		m_pVertexBuffer = new CGLES3VertexBuffer(vertexFormat, vertexBinding, size, bDynamic);
		return m_pVertexBuffer->BufferData(0, size, data);
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

	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_draws.clear();
}
