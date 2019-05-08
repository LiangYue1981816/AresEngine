#include "VKRenderer.h"


CVKMeshDraw::CVKMeshDraw(CVKDevice* pDevice, CVKMeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pIndirectBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
{
	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(nameDraw);
	m_pIndirectBuffer = new CVKIndirectBuffer(pDevice, 1);
	m_pInstanceBuffer = new CVKInstanceBuffer(pDevice, instanceFormat, instanceBinding);
}

CVKMeshDraw::~CVKMeshDraw(void)
{
	delete m_pIndirectBuffer;
	delete m_pInstanceBuffer;
}

void CVKMeshDraw::Release(void)
{
	m_pManager->Destroy(this);
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

GfxIndexType CVKMeshDraw::GetIndexType(void) const
{
	if (m_ptrMesh) {
		return m_ptrMesh->GetIndexBuffer()->GetIndexType();
	}
	else {
		return GFX_INDEX_INVALID_ENUM;
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

uint32_t CVKMeshDraw::GetVertexFormat(void) const
{
	if (m_ptrMesh) {
		return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
	}
	else {
		return 0;
	}
}

uint32_t CVKMeshDraw::GetVertexCount(void) const
{
	if (m_ptrMesh) {
		return m_ptrMesh->GetVertexBuffer()->GetVertexCount();
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

uint32_t CVKMeshDraw::GetInstanceCount(void) const
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->GetInstanceCount();
	}
	else {
		return 0;
	}
}

bool CVKMeshDraw::InstanceBufferData(size_t size, const void* data)
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->BufferData(size, data);
	}
	else {
		return false;
	}
}

void CVKMeshDraw::Bind(VkCommandBuffer vkCommandBuffer)
{
	if (m_ptrMesh) {
		((CVKIndexBuffer*)m_ptrMesh->GetIndexBuffer())->Bind(vkCommandBuffer);
		((CVKVertexBuffer*)m_ptrMesh->GetVertexBuffer())->Bind(vkCommandBuffer);
		((CVKVertexBuffer*)m_pInstanceBuffer)->Bind(vkCommandBuffer);
	}
}
