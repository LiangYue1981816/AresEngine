#include "VKRenderer.h"


CVKMeshDraw::CVKMeshDraw(CVKDevice* pDevice, CVKMeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
	: CGfxMeshDraw(name, ptrMesh, nameDraw, instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_mask(0)
	, m_pMeshDraw(nullptr)
	, m_pInstanceBuffer{ nullptr }

	, m_pRenderCallback(nullptr)
	, m_pRenderCallbackParam(nullptr)
{
	ASSERT(ptrMesh);
	ASSERT(ptrMesh->GetDraw(nameDraw));

	m_ptrMesh = ptrMesh;
	m_pMeshDraw = ptrMesh->GetDraw(nameDraw);

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_pInstanceBuffer[indexFrame] = new CVKInstanceBuffer(pDevice, instanceFormat, instanceBinding);
	}
}

CVKMeshDraw::~CVKMeshDraw(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		delete m_pInstanceBuffer[indexFrame];
	}
}

void CVKMeshDraw::Release(void)
{
	m_pManager->Destroy(this);
}

void CVKMeshDraw::SetMask(uint32_t mask)
{
	m_mask = mask;
}

uint32_t CVKMeshDraw::GetMask(void) const
{
	return m_mask;
}

CGfxMeshPtr CVKMeshDraw::GetMesh(void) const
{
	return m_ptrMesh;
}

glm::aabb CVKMeshDraw::GetLocalAABB(void) const
{
	return m_pMeshDraw->aabb;
}

uint32_t CVKMeshDraw::GetIndexCount(void) const
{
	return m_pMeshDraw->indexCount;
}

uint32_t CVKMeshDraw::GetIndexFirst(void) const
{
	return m_pMeshDraw->firstIndex;
}

uint32_t CVKMeshDraw::GetIndexOffset(void) const
{
	switch ((int)m_ptrMesh->GetIndexBuffer()->GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pMeshDraw->firstIndex * 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pMeshDraw->firstIndex * 4;
	default:                       return 0;
	}
}

uint32_t CVKMeshDraw::GetVertexBinding(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexBinding();
}

uint32_t CVKMeshDraw::GetVertexFormat(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexFormat();
}

uint32_t CVKMeshDraw::GetVertexCount(void) const
{
	return m_ptrMesh->GetVertexBuffer()->GetVertexCount();
}

uint32_t CVKMeshDraw::GetInstanceBinding(void) const
{
	return m_pInstanceBuffer[VKRenderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceBinding();
}

uint32_t CVKMeshDraw::GetInstanceFormat(void) const
{
	return m_pInstanceBuffer[VKRenderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceFormat();
}

uint32_t CVKMeshDraw::GetInstanceCount(void) const
{
	return m_pInstanceBuffer[VKRenderer()->GetSwapChain()->GetFrameIndex()]->GetInstanceCount();
}

bool CVKMeshDraw::InstanceBufferData(size_t size, const void* data)
{
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pInstanceBuffer[VKRenderer()->GetSwapChain()->GetFrameIndex()]->BufferData(size, data));
	return true;
}

void CVKMeshDraw::SetRenderCallback(RenderCallback callback, void* param)
{
	m_pRenderCallback = callback;
	m_pRenderCallbackParam = param;
}

void CVKMeshDraw::OnRenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) const
{
	if (m_pRenderCallback) {
		m_pRenderCallback(ptrCommandBuffer, m_pRenderCallbackParam);
	}
}

void CVKMeshDraw::Bind(VkCommandBuffer vkCommandBuffer)
{
	m_pInstanceBuffer[VKRenderer()->GetSwapChain()->GetFrameIndex()]->Bind(vkCommandBuffer);
}
