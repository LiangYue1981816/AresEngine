#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pManager(pManager)

	, m_mask(0)

	, m_pMeshDraw(nullptr)
	, m_pMultiInstanceBuffer{ nullptr }

	, m_pRenderCallback(nullptr)
	, m_pRenderCallbackParam(nullptr)
{
	ASSERT(ptrMesh);
	ASSERT(ptrMesh->GetDraw(nameDraw));

	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(nameDraw);
	m_pMultiInstanceBuffer = new CGLES3MultiInstanceBuffer(nullptr, instanceFormat, instanceBinding, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT);
}

CGLES3MeshDraw::~CGLES3MeshDraw(void)
{
	delete m_pMultiInstanceBuffer;
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

CGfxMeshPtr CGLES3MeshDraw::GetMesh(void) const
{
	return m_ptrMesh;
}

glm::aabb CGLES3MeshDraw::GetAABB(void) const
{
	return m_pMeshDraw->aabb;
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

uint32_t CGLES3MeshDraw::GetInstanceBinding(void) const
{
	return m_pMultiInstanceBuffer->GetBuffer(GLES3Renderer()->GetSwapChain()->GetFrameIndex())->GetInstanceBinding();
}

uint32_t CGLES3MeshDraw::GetInstanceFormat(void) const
{
	return m_pMultiInstanceBuffer->GetBuffer(GLES3Renderer()->GetSwapChain()->GetFrameIndex())->GetInstanceFormat();
}

uint32_t CGLES3MeshDraw::GetInstanceCount(void) const
{
	return m_pMultiInstanceBuffer->GetBuffer(GLES3Renderer()->GetSwapChain()->GetFrameIndex())->GetInstanceCount();
}

bool CGLES3MeshDraw::InstanceBufferData(size_t size, const void* data)
{
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMultiInstanceBuffer->GetBuffer(GLES3Renderer()->GetSwapChain()->GetFrameIndex())->BufferData(size, data));
	return true;
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

void CGLES3MeshDraw::Bind(void) const
{
	((CGLES3InstanceBuffer*)m_pMultiInstanceBuffer->GetBuffer(GLES3Renderer()->GetSwapChain()->GetFrameIndex()))->Bind();
}
