#include "GLES3Renderer.h"


CGLES3VertexArrayObject::CGLES3VertexArrayObject(void)
	: m_vao(0)
{
	glGenVertexArrays(1, &m_vao);
}

CGLES3VertexArrayObject::~CGLES3VertexArrayObject(void)
{
	glDeleteVertexArrays(1, &m_vao);
}

bool CGLES3VertexArrayObject::Buffer(CGLES3IndexBuffer *pIndexBuffer, CGLES3VertexBuffer *pVertexBuffer, CGLES3InstanceBuffer *pInstanceBuffer)
{
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
			pInstanceBuffer->Bind(nullptr);
		}
	}
	GLBindVertexArray(0);
	GLBindBuffer(GL_ARRAY_BUFFER, 0);
	GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void CGLES3VertexArrayObject::Bind(void *pParam)
{
	GLBindVertexArray(m_vao);
}
