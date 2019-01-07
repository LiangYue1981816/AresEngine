#include "VKRenderer.h"


CVKMeshDraw::CVKMeshDraw(CVKMeshDrawManager *pManager, uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, indexDraw, instanceFormat, instanceBinding)
	, m_name(name)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pIndirectBuffer(nullptr)
{
	if (ptrMesh.IsValid() && ptrMesh->GetDraw(indexDraw) != nullptr) {
		m_ptrMesh = ptrMesh;
		m_pMeshDraw = ptrMesh->GetDraw(indexDraw);

		m_pIndexBuffer = (CVKIndexBuffer *)ptrMesh->GetIndexBuffer();
		m_pVertexBuffer = (CVKVertexBuffer *)ptrMesh->GetVertexBuffer();

		m_pInstanceBuffer = new CVKInstanceBuffer(m_pManager->GetDevice(), instanceFormat, instanceBinding);

		m_pIndirectBuffer = new CVKIndirectBuffer(m_pManager->GetDevice(), 1);
		m_pIndirectBuffer->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, 0);
	}
}

CVKMeshDraw::~CVKMeshDraw(void)
{
	if (m_pIndirectBuffer) {
		delete m_pIndirectBuffer;
	}

	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	m_pIndirectBuffer = nullptr;
	m_pInstanceBuffer = nullptr;
}

void CVKMeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CVKMeshDraw::GetName(void) const
{
	return m_name;
}

bool CVKMeshDraw::InstanceBufferData(size_t size, const void *pBuffer)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->BufferData(size, pBuffer);

		if (m_pIndirectBuffer) {
			m_pIndirectBuffer->BufferData(0, size / GetInstanceStride(m_pInstanceBuffer->GetInstanceFormat()));
		}

		return true;
	}
	else {
		return false;
	}
}

GfxIndexType CVKMeshDraw::GetIndexType(void) const
{
	if (m_ptrMesh.IsValid()) {
		return m_ptrMesh->GetIndexType();
	}
	else {
		return GFX_INDEX_INVALID_ENUM;
	}
}

uint32_t CVKMeshDraw::GetVertexFormat(void) const
{
	if (m_ptrMesh.IsValid()) {
		return m_ptrMesh->GetVertexFormat();
	}
	else {
		return 0;
	}
}

uint32_t CVKMeshDraw::GetIndexCount(void) const
{
	if (m_pMeshDraw) {
		return m_pMeshDraw->indexCount;
	}
	else {
		return 0;
	}
}

uint32_t CVKMeshDraw::GetIndexOffset(void) const
{
	if (m_pMeshDraw) {
		switch ((int)GetIndexType()) {
		case GFX_INDEX_UNSIGNED_SHORT: return m_pMeshDraw->firstIndex * 2;
		case GFX_INDEX_UNSIGNED_INT:   return m_pMeshDraw->firstIndex * 4;
		default:                       return 0;
		}
	}
	else {
		return 0;
	}
}

uint32_t CVKMeshDraw::GetInstanceCount(void) const
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->GetInstanceCount();
	}
	else {
		return 0;
	}
}

uint32_t CVKMeshDraw::GetInstanceFormat(void) const
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->GetInstanceFormat();
	}
	else {
		return 0;
	}
}

glm::aabb CVKMeshDraw::GetLocalAABB(void) const
{
	if (m_pMeshDraw) {
		return m_pMeshDraw->aabb;
	}
	else {
		return glm::aabb();
	}
}

void CVKMeshDraw::Bind(VkCommandBuffer vkCommandBuffer, CVKBufferPtr &ptrIndexBuffer, CVKBufferPtr &ptrVertexBuffer, CVKBufferPtr &ptrInstanceBuffer)
{
	m_pIndexBuffer->Bind(vkCommandBuffer, 0, ptrIndexBuffer);
	m_pVertexBuffer->Bind(vkCommandBuffer, 0, ptrVertexBuffer);
	m_pInstanceBuffer->Bind(vkCommandBuffer, 0, ptrInstanceBuffer);
}
