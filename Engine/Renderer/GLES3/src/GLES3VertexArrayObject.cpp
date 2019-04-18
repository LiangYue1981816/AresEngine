#include "GLES3Renderer.h"


CGLES3VertexArrayObject::CGLES3VertexArrayObject(CGLES3IndexBuffer *pIndexBuffer, CGLES3VertexBuffer *pVertexBuffer, CGLES3InstanceBuffer *pInstanceBuffer)
	: m_vao(0)
{
	glGenVertexArrays(1, &m_vao);

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

CGLES3VertexArrayObject::~CGLES3VertexArrayObject(void)
{
	glDeleteVertexArrays(1, &m_vao);
}

void CGLES3VertexArrayObject::Bind(void) const
{
	GLBindVertexArray(m_vao);
}
