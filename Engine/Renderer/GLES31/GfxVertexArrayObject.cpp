#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxVertexArrayObject.h"


CGfxVertexArrayObject::CGfxVertexArrayObject(const CGfxIndexBuffer *pIndexBuffer, const CGfxVertexBuffer *pVertexBuffer, const CGfxInstanceBuffer *pInstanceBuffer)
	: m_vao(0)
{
	glGenVertexArrays(1, &m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(m_vao);
	{
		pIndexBuffer->Bind();
		pVertexBuffer->Bind();
		pInstanceBuffer->Bind();
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CGfxVertexArrayObject::~CGfxVertexArrayObject(void)
{
	glDeleteVertexArrays(1, &m_vao);
}

void CGfxVertexArrayObject::Bind(void) const
{
	glBindVertexArray(m_vao);
}
