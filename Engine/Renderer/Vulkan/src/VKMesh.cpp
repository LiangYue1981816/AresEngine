#include "VKRenderer.h"


CVKMesh::CVKMesh(CVKDevice* pDevice, CVKMeshManager* pManager, uint32_t name)
	: CGfxMesh(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
{

}

CVKMesh::~CVKMesh(void)
{
	Destroy();
}

void CVKMesh::Release(void)
{
	m_pManager->Destroy(this);
}

CGfxMesh::Draw* CVKMesh::GetDraw(uint32_t name)
{
	const auto& itDraw = m_draws.find(name);

	if (itDraw != m_draws.end()) {
		return &itDraw->second;
	}
	else {
		return nullptr;
	}
}

CGfxIndexBuffer* CVKMesh::GetIndexBuffer(void)
{
	return m_pIndexBuffer;
}

CGfxVertexBuffer* CVKMesh::GetVertexBuffer(void)
{
	return m_pVertexBuffer;
}

bool CVKMesh::CreateDraw(uint32_t name, const glm::aabb& aabb, int baseVertex, int firstIndex, int indexCount)
{
	m_draws[name].aabb = aabb;
	m_draws[name].baseVertex = baseVertex;
	m_draws[name].firstIndex = firstIndex;
	m_draws[name].indexCount = indexCount;
	return true;
}

bool CVKMesh::CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void* data)
{
	if (m_pIndexBuffer == nullptr) {
		m_pIndexBuffer = new CVKIndexBuffer(m_pDevice, type, size, bDynamic);
		return m_pIndexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

bool CVKMesh::CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data)
{
	if (m_pVertexBuffer == nullptr) {
		m_pVertexBuffer = new CVKVertexBuffer(m_pDevice, vertexFormat, vertexBinding, size, bDynamic);
		return m_pVertexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

void CVKMesh::Destroy(void)
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
