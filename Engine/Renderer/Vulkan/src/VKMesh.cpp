#include "VKRenderer.h"


CVKMesh::CVKMesh(CVKDevice* pDevice, CVKMeshManager* pManager, uint32_t name)
	: CGfxMesh(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
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

CGfxIndexBufferPtr CVKMesh::GetIndexBufferPtr(void)
{
	ASSERT(m_ptrIndexBuffer);
	return m_ptrIndexBuffer;
}

CGfxVertexBufferPtr CVKMesh::GetVertexBufferPtr(void)
{
	ASSERT(m_ptrVertexBuffer);
	return m_ptrVertexBuffer;
}

bool CVKMesh::CreateDraw(uint32_t name, const glm::aabb& aabb, int baseVertex, int firstIndex, int indexCount)
{
	m_draws[name].Set(aabb, baseVertex, firstIndex, indexCount);
	return true;
}

bool CVKMesh::CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void* data)
{
	if (m_ptrIndexBuffer == nullptr) {
		m_ptrIndexBuffer = CGfxIndexBufferPtr(new CVKIndexBuffer(m_pDevice, type, size, bDynamic));
		return m_ptrIndexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

bool CVKMesh::CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data)
{
	if (m_ptrVertexBuffer == nullptr) {
		m_ptrVertexBuffer = CGfxVertexBufferPtr(new CVKVertexBuffer(m_pDevice, vertexFormat, vertexBinding, size, bDynamic));
		return m_ptrVertexBuffer->BufferData(0, size, data);
	}
	else {
		return false;
	}
}

void CVKMesh::Destroy(void)
{
	m_draws.clear();
	m_ptrIndexBuffer.Release();
	m_ptrVertexBuffer.Release();
}
