#include "GLES3Renderer.h"


CGLES3MeshDraw::CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pManager(pManager)

	, m_pMeshDraw(nullptr)
	, m_pIndirectBuffer{ nullptr }
	, m_pInstanceBuffer{ nullptr }
	, m_pVertexArrayObject{ nullptr }
{
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

glm::aabb CGLES3MeshDraw::GetLocalAABB(void) const
{
	ASSERT(m_pMeshDraw);
	return m_pMeshDraw->aabb;
}

GfxIndexType CGLES3MeshDraw::GetIndexType(void) const
{
	ASSERT(m_ptrMesh);
	return m_ptrMesh->GetIndexBuffer()->GetIndexType();
}

uint32_t CGLES3MeshDraw::GetIndexCount(void) const
{
	ASSERT(m_pMeshDraw);
	return m_pMeshDraw->indexCount;
}

uint32_t CGLES3MeshDraw::GetIndexOffset(void) const
{
	ASSERT(m_pMeshDraw);

	switch ((int)GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pMeshDraw->firstIndex * 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pMeshDraw->firstIndex * 4;
	default:                       return 0;
	}
}

uint32_t CGLES3MeshDraw::GetVertexFormat(void) const
{
	ASSERT(m_ptrMesh);
	return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
}

uint32_t CGLES3MeshDraw::GetVertexCount(void) const
{
	ASSERT(m_ptrMesh);
	return m_ptrMesh->GetVertexBuffer()->GetVertexCount();
}

uint32_t CGLES3MeshDraw::GetInstanceFormat(void) const
{
	ASSERT(m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]);
	return m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceFormat();
}

uint32_t CGLES3MeshDraw::GetInstanceCount(void) const
{
	ASSERT(m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]);
	return m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceCount();
}

bool CGLES3MeshDraw::InstanceBufferData(size_t size, const void* data)
{
	ASSERT(m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]);
	ASSERT(m_pIndirectBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]);

	m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->BufferData(size, data);
	m_pIndirectBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->BufferData(0, m_pMeshDraw->baseVertex, m_pMeshDraw->firstIndex, m_pMeshDraw->indexCount, size / GetInstanceStride(m_pInstanceBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceFormat()));

	return true;
}

void CGLES3MeshDraw::Bind(void) const
{
	ASSERT(m_pIndirectBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]);
	ASSERT(m_pVertexArrayObject[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]);

	m_pIndirectBuffer[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->Bind();
	m_pVertexArrayObject[GLES3Renderer()->GetSwapChain()->GetFrameIndex()]->Bind();
}
