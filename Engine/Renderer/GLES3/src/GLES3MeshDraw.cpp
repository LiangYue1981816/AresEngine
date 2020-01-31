#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw)
	: CGfxMeshDraw(name, ptrMesh, nameDraw)
	, m_pManager(pManager)
	, m_pMeshDraw(nullptr)

	, m_mask(0)

	, m_pRenderCallback(nullptr)
	, m_pRenderCallbackParam(nullptr)
{
	ASSERT(ptrMesh);
	ASSERT(ptrMesh->GetDraw(nameDraw));

	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(nameDraw);
}

CGLES3MeshDraw::~CGLES3MeshDraw(void)
{

}

void CGLES3MeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

void CGLES3MeshDraw::SetMask(uint32_t mask)
{
	m_mask = mask;
}

uint32_t CGLES3MeshDraw::GetMask(void) const
{
	return m_mask;
}

glm::aabb CGLES3MeshDraw::GetAABB(void) const
{
	return m_pMeshDraw->aabb;
}

CGfxMeshPtr CGLES3MeshDraw::GetMesh(void) const
{
	return m_ptrMesh;
}

uint32_t CGLES3MeshDraw::GetIndexType(void) const
{
	return m_ptrMesh->GetIndexBufferPtr()->GetIndexType();
}

uint32_t CGLES3MeshDraw::GetIndexCount(void) const
{
	return m_pMeshDraw->indexCount;
}

uint32_t CGLES3MeshDraw::GetIndexFirst(void) const
{
	return m_pMeshDraw->firstIndex;
}

uint32_t CGLES3MeshDraw::GetIndexOffset(void) const
{
	switch (GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pMeshDraw->firstIndex * 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pMeshDraw->firstIndex * 4;
	default:                       return 0;
	}
}

uint32_t CGLES3MeshDraw::GetVertexBinding(void) const
{
	return m_ptrMesh->GetVertexBufferPtr()->GetVertexBinding();
}

uint32_t CGLES3MeshDraw::GetVertexFormat(void) const
{
	return m_ptrMesh->GetVertexBufferPtr()->GetVertexFormat();
}

uint32_t CGLES3MeshDraw::GetVertexCount(void) const
{
	return m_ptrMesh->GetVertexBufferPtr()->GetVertexCount();
}

void CGLES3MeshDraw::SetRenderCallback(RenderCallback callback, void* param)
{
	m_pRenderCallback = callback;
	m_pRenderCallbackParam = param;
}

void CGLES3MeshDraw::OnRenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) const
{
	if (m_pRenderCallback) {
		m_pRenderCallback(ptrCommandBuffer, m_pRenderCallbackParam);
	}
}
