#include "GLES3Renderer.h"


CGLES3Buffer::CGLES3Buffer(uint32_t target, size_t size, bool bDynamic)
	: m_target(0)
	, m_buffer(0)
	, m_size(0)
{
	Create(target, size, bDynamic);
}

CGLES3Buffer::~CGLES3Buffer(void)
{
	Destroy();
}

bool CGLES3Buffer::Create(uint32_t target, size_t size, bool bDynamic)
{
	ASSERT(size);
	ASSERT(
		target == GL_ARRAY_BUFFER ||
		target == GL_COPY_READ_BUFFER ||
		target == GL_COPY_WRITE_BUFFER ||
		target == GL_ELEMENT_ARRAY_BUFFER ||
		target == GL_PIXEL_PACK_BUFFER ||
		target == GL_PIXEL_UNPACK_BUFFER ||
		target == GL_TRANSFORM_FEEDBACK_BUFFER ||
		target == GL_UNIFORM_BUFFER ||
		target == GL_ATOMIC_COUNTER_BUFFER ||
		target == GL_DRAW_INDIRECT_BUFFER ||
		target == GL_DISPATCH_INDIRECT_BUFFER ||
		target == GL_SHADER_STORAGE_BUFFER);

	m_target = target;
	m_size = size;

	glGenBuffers(1, &m_buffer);
	glBindBuffer(m_target, m_buffer);
	glBufferData(m_target, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(m_target, 0);
	CHECK_GL_ERROR_ASSERT();

	return true;
}

void CGLES3Buffer::Destroy(void)
{
	ASSERT(m_buffer);

	glDeleteBuffers(1, &m_buffer);

	m_target = 0;
	m_buffer = 0;
	m_size = 0;
}

uint32_t CGLES3Buffer::GetTarget(void) const
{
	ASSERT(m_target);
	return m_target;
}

uint32_t CGLES3Buffer::GetBuffer(void) const
{
	ASSERT(m_buffer);
	return m_buffer;
}

uint32_t CGLES3Buffer::GetSize(void) const
{
	ASSERT(m_size);
	return m_size;
}

bool CGLES3Buffer::BufferSize(size_t size, bool bDynamic)
{
	ASSERT(size);
	ASSERT(m_buffer);

	m_size = size;
	glBindBuffer(m_target, m_buffer);
	glBufferData(m_target, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(m_target, 0);
	CHECK_GL_ERROR_ASSERT();

	return true;
}

bool CGLES3Buffer::BufferData(size_t offset, size_t size, const void* data, bool bSync)
{
	ASSERT(data);
	ASSERT(size);
	ASSERT(m_size >= (uint32_t)(offset + size));
	ASSERT(m_buffer);

	glBindBuffer(m_target, m_buffer);
	glBufferSubDataSync(m_target, offset, size, data, bSync);
	glBindBuffer(m_target, 0);
	CHECK_GL_ERROR_ASSERT();

	return true;
}

void CGLES3Buffer::Bind(void) const
{
	ASSERT(m_buffer);

	GLBindBuffer(m_target, m_buffer);
	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Buffer::BindRange(int binding, size_t offset, size_t range) const
{
	ASSERT(m_buffer);

	GLBindBufferRange(m_target, binding, m_buffer, offset, range);
	CHECK_GL_ERROR_ASSERT();
}
