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

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
		return false;
	}

	if (m_draws[indexDraw].baseVertex == baseVertex &&
		m_draws[indexDraw].firstIndex == firstIndex &&
		m_draws[indexDraw].indexCount == indexCount &&
		m_draws[indexDraw].instanceCount == instanceCount) {
		return true;
	}

	m_draws[indexDraw].baseVertex = baseVertex;
	m_draws[indexDraw].firstIndex = firstIndex;
	m_draws[indexDraw].indexCount = indexCount;
	m_draws[indexDraw].instanceCount = instanceCount;

	return m_pBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw], false);
}

void CGLES3IndirectBuffer::Bind(void) const
{
	m_pBuffer->Bind();
}
