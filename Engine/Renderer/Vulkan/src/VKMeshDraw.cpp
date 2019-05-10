#include "VKRenderer.h"


CVKMeshDraw::CVKMeshDraw(CVKDevice* pDevice, CVKMeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pIndirectBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
{
	ASSERT(m_pDevice);
	ASSERT(m_pManager);
	ASSERT(ptrMesh);
	ASSERT(ptrMesh->GetDraw(nameDraw));

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
	return m_pMeshDraw->aabb;
}

GfxIndexType CVKMeshDraw::GetIndexType(void) const
{
	return m_ptrMesh->GetIndexBuffer()->GetIndexType();
}

uint32_t CVKMeshDraw::GetIndexCount(void) const
{
	return m_pMeshDraw->indexCount;
}

uint32_t CVKMeshDraw::GetIndexOffset(void) const
{
	switch ((int)GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pMeshDraw->firstIndex * 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pMeshDraw->firstIndex * 4;
	default:                       return 0;
	}
}

uint32_t CVKMeshDraw::GetVertexFormat(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
}

uint32_t CVKMeshDraw::GetVertexCount(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexCount();
}

uint32_t CVKMeshDraw::GetInstanceFormat(void) const
{
	return m_pInstanceBuffer->GetInstanceFormat();
}

uint32_t CVKMeshDraw::GetInstanceCount(void) const
{
	return m_pInstanceBuffer->GetInstanceCount();
}

bool CVKMeshDraw::InstanceBufferData(size_t size, const void* data)
{
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pInstanceBuffer->BufferData(size, data));
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pIndirectBuffer->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, size / GetInstanceStride(m_pInstanceBuffer->GetInstanceFormat())));
	return true;
}

void CVKMeshDraw::Bind(VkCommandBuffer vkCommandBuffer)
{
	ASSERT(vkCommandBuffer);
	((CVKIndexBuffer*)m_ptrMesh->GetIndexBuffer())->Bind(vkCommandBuffer);
	((CVKVertexBuffer*)m_ptrMesh->GetVertexBuffer())->Bind(vkCommandBuffer);
	((CVKVertexBuffer*)m_pInstanceBuffer)->Bind(vkCommandBuffer);
}
