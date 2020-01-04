#include "GLES3Renderer.h"


CGLES3IndirectBuffer::CGLES3IndirectBuffer(int numDrawCommands)
	: CGfxIndirectBuffer(numDrawCommands)
	, m_pBuffer(nullptr)

	, m_draws(numDrawCommands)
{
	m_pBuffer = new CGLES3Buffer(GL_DRAW_INDIRECT_BUFFER, numDrawCommands * sizeof(DrawCommand), true);
	CGfxProfiler::IncIndirectBufferSize(m_pBuffer->GetSize());
}

CGLES3IndirectBuffer::~CGLES3IndirectBuffer(void)
{
	CGfxProfiler::DecIndirectBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CGLES3IndirectBuffer::Release(void)
{
	delete this;
}

uint32_t CGLES3IndirectBuffer::GetDrawCommandCount(void) const
{
	return m_draws.size();
}

uint32_t CGLES3IndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return indexDraw * sizeof(DrawCommand);
}

uint32_t CGLES3IndirectBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

uint32_t CGLES3IndirectBuffer::GetStride(void) const
{
	return sizeof(DrawCommand);
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
		return false;
	}

	if (m_draws[indexDraw].indexCount == indexCount &&
		m_draws[indexDraw].instanceCount == instanceCount &&
		m_draws[indexDraw].firstIndex == firstIndex &&
		m_draws[indexDraw].baseVertex == baseVertex &&
		m_draws[indexDraw].baseInstance == baseInstance) {
		return true;
	}

	m_draws[indexDraw].indexCount = indexCount;
	m_draws[indexDraw].instanceCount = instanceCount;
	m_draws[indexDraw].firstIndex = firstIndex;
	m_draws[indexDraw].baseVertex = baseVertex;
	m_draws[indexDraw].baseInstance = baseInstance;

	return m_pBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw], false);
}

void CGLES3IndirectBuffer::Bind(void) const
{
	m_pBuffer->Bind();
}


CGLES3MultiIndirectBuffer::CGLES3MultiIndirectBuffer(int numDrawCommands, int count)
	: CGfxMultiIndirectBuffer(numDrawCommands, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CGLES3IndirectBuffer(numDrawCommands);
	}
}

CGLES3MultiIndirectBuffer::~CGLES3MultiIndirectBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CGLES3MultiIndirectBuffer::Release(void)
{
	delete this;
}

bool CGLES3MultiIndirectBuffer::SetIndex(int index)
{
	if (index >= 0 && index < m_pBuffers.size()) {
		m_index = index;
		return true;
	}
	else {
		return false;
	}
}

CGLES3IndirectBuffer* CGLES3MultiIndirectBuffer::GetBuffer(void) const
{
	return m_pBuffers[m_index];
}

CGLES3IndirectBuffer* CGLES3MultiIndirectBuffer::GetBuffer(int index) const
{
	if (index >= 0 && index < m_pBuffers.size()) {
		return m_pBuffers[index];
	}
	else {
		return nullptr;
	}
}

uint32_t CGLES3MultiIndirectBuffer::GetDrawCommandCount(void) const
{
	return m_pBuffers[m_index]->GetDrawCommandCount();
}

uint32_t CGLES3MultiIndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return m_pBuffers[m_index]->GetDrawCommandOffset(indexDraw);
}

uint32_t CGLES3MultiIndirectBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

uint32_t CGLES3MultiIndirectBuffer::GetStride(void) const
{
	return m_pBuffers[m_index]->GetStride();
}

bool CGLES3MultiIndirectBuffer::BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount)
{
	return m_pBuffers[m_index]->BufferData(indexDraw, firstIndex, baseVertex, baseInstance, indexCount, instanceCount);
}

void CGLES3MultiIndirectBuffer::Bind(void) const
{
	m_pBuffers[m_index]->Bind();
}
