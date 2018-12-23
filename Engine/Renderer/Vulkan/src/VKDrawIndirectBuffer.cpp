#include "VKRenderer.h"


CVKDrawIndirectBuffer::CVKDrawIndirectBuffer(CVKDevice *pDevice, uint32_t count)
	: CVKBufferBase(pDevice, count * sizeof(DrawCommand), VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	, CGfxDrawIndirectBuffer(count)
{
	m_draws.resize(m_count);
}

CVKDrawIndirectBuffer::~CVKDrawIndirectBuffer(void)
{

}

bool CVKDrawIndirectBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

bool CVKDrawIndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;
		return CVKBufferBase::BufferData(indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
	}

	return true;
}

bool CVKDrawIndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
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
		return CVKBufferBase::BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}

	return true;
}

void CVKDrawIndirectBuffer::Bind(void *pParam)
{

}
