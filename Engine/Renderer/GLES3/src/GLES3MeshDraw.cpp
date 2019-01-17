#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager *pManager, uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, indexDraw, instanceFormat, instanceBinding)
	, m_name(name)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pIndirectBuffer(nullptr)
	, m_pVertexArrayObject(nullptr)
{
	if (ptrMesh.IsValid() && ptrMesh->GetDraw(indexDraw) != nullptr) {
		m_ptrMesh = ptrMesh;
		m_pMeshDraw = ptrMesh->GetDraw(indexDraw);

		m_pInstanceBuffer = new CGLES3InstanceBuffer(instanceFormat, instanceBinding);

		m_pIndirectBuffer = new CGLES3IndirectBuffer(1);
		m_pIndirectBuffer->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, 0);

		m_pVertexArrayObject = new CGLES3VertexArrayObject;
		m_pVertexArrayObject->Buffer((CGLES3IndexBuffer *)m_ptrMesh->GetIndexBuffer(), (CGLES3VertexBuffer *)m_ptrMesh->GetVertexBuffer(), m_pInstanceBuffer);
	}
}

CGLES3MeshDraw::~CGLES3MeshDraw(void)
{
	if (m_pIndirectBuffer) {
		delete m_pIndirectBuffer;
	}

	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pIndirectBuffer = nullptr;
	m_pInstanceBuffer = nullptr;
	m_pVertexArrayObject = nullptr;
}

void CGLES3MeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3MeshDraw::GetName(void) const
{
	return m_name;
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
	if (m_ptrMesh.IsValid()) {
		return m_ptrMesh->GetIndexType();
	}
	else {
		return GFX_INDEX_INVALID_ENUM;
	}
}

uint32_t CGLES3MeshDraw::GetVertexFormat(void) const
{
	if (m_ptrMesh.IsValid()) {
		return m_ptrMesh->GetVertexFormat();
	}
	else {
		return 0;
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

uint32_t CGLES3MeshDraw::GetInstanceCount(void) const
{
	if (m_pInstanceBuffer) {
		return m_pInstanceBuffer->GetInstanceCount();
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

bool CGLES3MeshDraw::InstanceBufferData(size_t size, const void *pBuffer)
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

void CGLES3MeshDraw::Bind(void)
{
	if (m_pVertexArrayObject) {
		m_pVertexArrayObject->Bind();
	}

	if (m_pIndirectBuffer) {
		m_pIndirectBuffer->Bind();
	}
}
