#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager *pManager, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceBinding, uint32_t instanceFormat)
	: CGfxMeshDraw(ptrMesh, indexDraw, instanceBinding, instanceFormat)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pVertexArrayObject(nullptr)
	, m_pDrawIndirectBuffer(nullptr)
{
	if (ptrMesh.IsValid() && ptrMesh->GetDraw(indexDraw)) {
		m_ptrMesh = ptrMesh;
		m_pMeshDraw = ptrMesh->GetDraw(indexDraw);

		m_pInstanceBuffer = new CGLES3InstanceBuffer(instanceBinding, instanceFormat);
		m_pVertexArrayObject = new CGLES3VertexArrayObject;
		m_pVertexArrayObject->Buffer((CGLES3IndexBuffer*)ptrMesh->GetIndexBuffer(), (CGLES3VertexBuffer *)ptrMesh->GetVertexBuffer(), m_pInstanceBuffer);

		m_pDrawIndirectBuffer = new CGLES3DrawIndirectBuffer(1);
		m_pDrawIndirectBuffer->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, 0);
	}
}

CGLES3MeshDraw::~CGLES3MeshDraw(void)
{
	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	if (m_pDrawIndirectBuffer) {
		delete m_pDrawIndirectBuffer;
	}

	m_pInstanceBuffer = nullptr;
	m_pVertexArrayObject = nullptr;
	m_pDrawIndirectBuffer = nullptr;
}

void CGLES3MeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3MeshDraw::InstanceBufferData(size_t size, const void *pBuffer)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->BufferData(size, pBuffer);
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
		case GFX_INDEX_UNSIGNED_BYTE:  return m_pMeshDraw->firstIndex;
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

void CGLES3MeshDraw::Bind(void *pParam)
{
	if (m_pVertexArrayObject) {
		m_pVertexArrayObject->Bind(nullptr);
	}

	if (m_pDrawIndirectBuffer) {
		m_pDrawIndirectBuffer->Bind(nullptr);
	}
}
