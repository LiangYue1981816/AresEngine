#include "GLES3Renderer.h"


CGLES3VertexArrayObject::CGLES3VertexArrayObject(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer)
	: m_vao(0)
{
	Create(pIndexBuffer, pVertexBuffer, pInstanceBuffer);
}

CGLES3VertexArrayObject::~CGLES3VertexArrayObject(void)
{
	Destroy();
}

bool CGLES3VertexArrayObject::Create(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer)
{
	ASSERT(pIndexBuffer);
	ASSERT(pVertexBuffer);
	ASSERT(pInstanceBuffer);

	glGenVertexArrays(1, &m_vao);
	{
		GLBindVertexArray(0);
		GLBindBuffer(GL_ARRAY_BUFFER, 0);
		GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		GLBindVertexArray(m_vao);
		{
			if (pIndexBuffer) {
				pIndexBuffer->Bind();
			}

			if (pVertexBuffer) {
				pVertexBuffer->Bind();
			}

			if (pInstanceBuffer) {
				pInstanceBuffer->Bind();
			}
		}
		GLBindVertexArray(0);
		GLBindBuffer(GL_ARRAY_BUFFER, 0);
		GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	CHECK_GL_ERROR_ASSERT();

	return true;
}

void CGLES3VertexArrayObject::Destroy(void)
{
	ASSERT(m_vao);

	glDeleteVertexArrays(1, &m_vao);
	m_vao = 0;
}

void CGLES3VertexArrayObject::Bind(void) const
{
	ASSERT(m_vao);

	GLBindVertexArray(m_vao);
	CHECK_GL_ERROR_ASSERT();
}
