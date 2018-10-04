#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformBuffer.h"


CGfxUniformBuffer::CGfxUniformBuffer(size_t size, bool bDynamic)
	: m_buffer(0)
	, m_size(size)
{
	CGfxProfiler::IncUniformBufferSize(m_size);

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

CGfxUniformBuffer::~CGfxUniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_size);

	glDeleteBuffers(1, &m_buffer);
	m_buffer = 0;
	m_size = 0;
}

bool CGfxUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, (int)offset, (uint32_t)size, pBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

uint32_t CGfxUniformBuffer::GetBuffer(void) const
{
	return m_buffer;
}

uint32_t CGfxUniformBuffer::GetSize(void) const
{
	return m_size;
}
