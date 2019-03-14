#include "VKRenderer.h"


CVKMeshDraw::CVKMeshDraw(CVKDevice *pDevice, CVKMeshDrawManager *pManager, uint32_t name, const CGfxMeshPtr ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, indexDraw, instanceFormat, instanceBinding)
	, m_name(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pIndirectBuffer(nullptr)
{
	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(indexDraw);

	m_pIndexBuffer = (CVKIndexBuffer *)ptrMesh->GetIndexBuffer();
	m_pVertexBuffer = (CVKVertexBuffer *)ptrMesh->GetVertexBuffer();

	m_pInstanceBuffer = new CVKInstanceBuffer(m_pDevice, instanceFormat, instanceBinding);

	m_pIndirectBuffer = new CVKIndirectBuffer(m_pDevice, 1);
	m_pIndirectBuffer->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, 0);
}

CVKMeshDraw::~CVKMeshDraw(void)
{
	delete m_pInstanceBuffer;
	delete m_pIndirectBuffer;
}

void CVKMeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CVKMeshDraw::GetName(void) const
{
	return m_name;
}

uint32_t CVKMeshDraw::GetVertexFormat(void) const
{
	if (m_ptrMesh.IsValid()) {
		return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
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

GfxIndexType CVKMeshDraw::GetIndexType(void) const
{
	if (m_ptrMesh.IsValid()) {
		return m_ptrMesh->GetIndexBuffer()->GetIndexType();
	}
	else {
		return GFX_INDEX_INVALID_ENUM;
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

bool CVKMeshDraw::InstanceBufferData(size_t size, const void *pBuffer)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->BufferData(size, pBuffer);
		m_pIndirectBuffer->BufferData(0, size / GetInstanceStride(m_pInstanceBuffer->GetInstanceFormat()));
		return true;
	}
	else {
		return false;
	}
}

void CVKMeshDraw::Bind(VkCommandBuffer vkCommandBuffer) const
{
	m_pIndexBuffer->Bind(vkCommandBuffer, 0);
	m_pVertexBuffer->Bind(vkCommandBuffer, 0);
	m_pInstanceBuffer->Bind(vkCommandBuffer, 0);
}

CVKBufferPtr CVKMeshDraw::IndexBufferTransfer(VkCommandBuffer vkCommandBuffer)
{
	return m_pIndexBuffer->BufferTransfer(vkCommandBuffer);
}

CVKBufferPtr CVKMeshDraw::VertexBufferTransfer(VkCommandBuffer vkCommandBuffer)
{
	return m_pVertexBuffer->BufferTransfer(vkCommandBuffer);
}
