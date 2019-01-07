#include "VKRenderer.h"


CVKMesh::CVKMesh(CVKMeshManager *pManager, uint32_t name)
	: CGfxMesh(name)
	, m_name(name)
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

uint32_t CVKMesh::GetName(void) const
{
	return m_name;
}

bool CVKMesh::CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer)
{
	if (m_pIndexBuffer == nullptr) {
		m_pIndexBuffer = new CVKIndexBuffer(m_pManager->GetDevice(), type, size, bDynamic);
		m_pIndexBuffer->BufferData(0, size, pBuffer);
		return true;
	}
	else {
		return false;
	}
}

bool CVKMesh::CreateVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic, const void *pBuffer)
{
	if (m_pVertexBuffer == nullptr) {
		m_pVertexBuffer = new CVKVertexBuffer(m_pManager->GetDevice(), vertexFormat, vertexBinding, size, bDynamic);
		m_pVertexBuffer->BufferData(0, size, pBuffer);
		return true;
	}
	else {
		return false;
	}
}

bool CVKMesh::CreateDraw(int indexDraw, const glm::aabb &aabb, int baseVertex, int firstIndex, int indexCount)
{
	m_draws[indexDraw].aabb = aabb;
	m_draws[indexDraw].baseVertex = baseVertex;
	m_draws[indexDraw].firstIndex = firstIndex;
	m_draws[indexDraw].indexCount = indexCount;
	return true;
}

void CVKMesh::Destroy(void)
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

CGfxIndexBuffer* CVKMesh::GetIndexBuffer(void)
{
	return m_pIndexBuffer;
}

CGfxVertexBuffer* CVKMesh::GetVertexBuffer(void)
{
	return m_pVertexBuffer;
}

CGfxMesh::Draw* CVKMesh::GetDraw(int indexDraw)
{
	const auto &itDraw = m_draws.find(indexDraw);

	if (itDraw != m_draws.end()) {
		return &itDraw->second;
	}
	else {
		return nullptr;
	}
}

GfxIndexType CVKMesh::GetIndexType(void) const
{
	if (m_pIndexBuffer) {
		return m_pIndexBuffer->GetIndexType();
	}
	else {
		return GFX_INDEX_INVALID_ENUM;
	}
}

uint32_t CVKMesh::GetVertexFormat(void) const
{
	if (m_pVertexBuffer) {
		return m_pVertexBuffer->GetVertexFormat();
	}
	else {
		return 0;
	}
}
