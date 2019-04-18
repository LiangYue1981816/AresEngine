#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pIndirectBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pVertexArrayObject(nullptr)
{
	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(nameDraw);

	m_pIndirectBuffer = new CGLES3IndirectBuffer(1);
	m_pInstanceBuffer = new CGLES3InstanceBuffer(instanceFormat, instanceBinding);
	m_pVertexArrayObject = new CGLES3VertexArrayObject((CGLES3IndexBuffer *)m_ptrMesh->GetIndexBuffer(), (CGLES3VertexBuffer *)m_ptrMesh->GetVertexBuffer(), m_pInstanceBuffer);
}

CGLES3MeshDraw::~CGLES3MeshDraw(void)
{
	delete m_pIndirectBuffer;
	delete m_pInstanceBuffer;
	delete m_pVertexArrayObject;
}

void CGLES3MeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

glm::aabb CGLES3MeshDraw::GetLocalAABB(void) const
{
	if (m_pMeshDraw) {
		return m_pMeshDraw->aabb;
	}
	else {
		return glm::aabb();
	}
}

GfxIndexType CGLES3MeshDraw::GetIndexType(void) const
{
	if (m_ptrMesh) {
		return m_ptrMesh->GetIndexBuffer()->GetIndexType();
	}
	else {
		return GFX_INDEX_INVALID_ENUM;
	}
}

uint32_t CGLES3MeshDraw::GetIndexCount(void) const
{
	if (m_pMeshDraw) {
		return m_pMeshDraw->indexCount;
	}
	else {
		return 0;
	}
}

uint32_t CGLES3MeshDraw::GetIndexOffset(void) const
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

uint32_t CGLES3MeshDraw::GetVertexFormat(void) const
{
	if (m_ptrMesh) {
		return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3MeshDraw::GetVertexCount(void) const
{
	if (m_ptrMesh) {
		return m_ptrMesh->GetVertexBuffer()->GetVertexCount();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3MeshDraw::GetInstanceFormat(void) const
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->GetInstanceFormat();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3MeshDraw::GetInstanceCount(void) const
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->GetInstanceCount();
	}
	else {
		return 0;
	}
}

bool CGLES3MeshDraw::InstanceBufferData(size_t size, const void *pBuffer)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->BufferData(size, pBuffer);
		m_pIndirectBuffer->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, size / GetInstanceStride(m_pInstanceBuffer->GetInstanceFormat()));
		return true;
	}
	else {
		return false;
	}
}

void CGLES3MeshDraw::Bind(void) const
{
	if (m_pVertexArrayObject) {
		m_pVertexArrayObject->Bind();
	}

	if (m_pIndirectBuffer) {
		m_pIndirectBuffer->Bind();
	}
}
