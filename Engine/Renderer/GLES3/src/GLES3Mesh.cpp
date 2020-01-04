#include "GLES3Renderer.h"


CGLES3Mesh::CGLES3Mesh(CGLES3MeshManager* pManager, uint32_t name)
	: CGfxMesh(name)
	, m_pManager(pManager)
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

CGfxIndexBufferPtr CGLES3Mesh::GetIndexBufferPtr(void)
{
	ASSERT(m_ptrIndexBuffer);
	return m_ptrIndexBuffer;
}

CGfxVertexBufferPtr CGLES3Mesh::GetVertexBufferPtr(void)
{
	ASSERT(m_ptrVertexBuffer);
	return m_ptrVertexBuffer;
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
	if (m_ptrIndexBuffer == nullptr) {
		m_ptrIndexBuffer = CGfxIndexBufferPtr(new CGLES3IndexBuffer(type, size, bDynamic));
		return m_ptrIndexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data)
{
	if (m_ptrVertexBuffer == nullptr) {
		m_ptrVertexBuffer = CGfxVertexBufferPtr(new CGLES3VertexBuffer(vertexFormat, vertexBinding, size, bDynamic));
		return m_ptrVertexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

void CGLES3Mesh::Destroy(void)
{
	m_draws.clear();
	m_ptrIndexBuffer.Release();
	m_ptrVertexBuffer.Release();
}
