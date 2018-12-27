#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice *pDevice, uint32_t count)
	: CGfxIndirectBuffer(count)
	, m_pDevice(pDevice)
{
	m_draws.resize(m_count);
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * m_size));
}

CVKIndirectBuffer::~CVKIndirectBuffer(void)
{

}

bool CVKIndirectBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return false;
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;
		return m_ptrBuffer->BufferData(indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
	}

	return true;
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].baseVertex != baseVertex ||
		m_draws[indexDraw].firstIndex != firstIndex ||
		m_draws[indexDraw].indexCount != indexCount ||
		m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].baseVertex = baseVertex;
		m_draws[indexDraw].firstIndex = firstIndex;
		m_draws[indexDraw].indexCount = indexCount;
		m_draws[indexDraw].instanceCount = instanceCount;
		return m_ptrBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}

	return true;
}

void CVKIndirectBuffer::Bind(void *pParam)
{

}

CVKBufferPtr CVKIndirectBuffer::GetBuffer(void) const
{
	return m_ptrBuffer;
}
