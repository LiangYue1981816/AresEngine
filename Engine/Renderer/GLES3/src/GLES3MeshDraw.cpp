#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pManager(pManager)

	, m_mask(0)
	, m_pMeshDraw(nullptr)
	, m_pIndirectBuffer{ nullptr }
	, m_pInstanceBuffer{ nullptr }
	, m_pVertexArrayObject{ nullptr }

	, m_pRenderCallback(nullptr)
	, m_pRenderCallbackParam(nullptr)
{
	ASSERT(ptrMesh);
	ASSERT(ptrMesh->GetDraw(nameDraw));

	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(nameDraw);

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_pIndirectBuffer[indexFrame] = new CGLES3IndirectBuffer(1);
		m_pInstanceBuffer[indexFrame] = new CGLES3InstanceBuffer(instanceFormat, instanceBinding);
		m_pVertexArrayObject[indexFrame] = new CGLES3VertexArrayObject((const CGLES3IndexBuffer*)m_ptrMesh->GetIndexBuffer(), (const CGLES3VertexBuffer*)m_ptrMesh->GetVertexBuffer(), (const CGLES3InstanceBuffer*)m_pInstanceBuffer[indexFrame]);
	}
}

CGLES3MeshDraw::~CGLES3MeshDraw(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		delete m_pIndirectBuffer[indexFrame];
		delete m_pInstanceBuffer[indexFrame];
		delete m_pVertexArrayObject[indexFrame];
	}
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

glm::aabb CGLES3MeshDraw::GetLocalAABB(void) const
{
	return m_pMeshDraw->aabb;
}

GfxIndexType CGLES3MeshDraw::GetIndexType(void) const
{
	return m_ptrMesh->GetIndexBuffer()->GetIndexType();
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
	switch ((int)GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pMeshDraw->firstIndex * 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pMeshDraw->firstIndex * 4;
	default:                       return 0;
	}
}

uint32_t CGLES3MeshDraw::GetVertexBinding(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexBinding();
}

uint32_t CGLES3MeshDraw::GetVertexFormat(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
}

uint32_t CGLES3MeshDraw::GetVertexCount(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexCount();
}

uint32_t CGLES3MeshDraw::GetInstanceBinding(void) const
{
	return m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceBinding();
}

uint32_t CGLES3MeshDraw::GetInstanceFormat(void) const
{
	return m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceFormat();
}

uint32_t CGLES3MeshDraw::GetInstanceCount(void) const
{
	return m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceCount();
}

bool CGLES3MeshDraw::InstanceBufferData(size_t size, const void* data)
{
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->BufferData(size, data));
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pIndirectBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, size / GetInstanceStride(m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceFormat())));
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
	m_pIndirectBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->Bind();
	m_pVertexArrayObject[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->Bind();
}
