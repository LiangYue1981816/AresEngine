#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxVertexArrayObject.h"


CGfxVertexArrayObject::CGfxVertexArrayObject(void)
	: m_vao(0)
{
	glGenVertexArrays(1, &m_vao);
}

CGfxVertexArrayObject::~CGfxVertexArrayObject(void)
{
	glDeleteVertexArrays(1, &m_vao);
}

bool CGfxVertexArrayObject::Buffer(CGfxIndexBuffer *pIndexBuffer, CGfxVertexBuffer *pVertexBuffer, CGfxInstanceBuffer *pInstanceBuffer) const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(m_vao);
	{
		if (pIndexBuffer) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer());
		}

		if (pVertexBuffer) {
			glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer());
			pVertexBuffer->SetupFormat();
		}

		if (pInstanceBuffer) {
			glBindBuffer(GL_ARRAY_BUFFER, pInstanceBuffer->GetInstanceBuffer());
			pInstanceBuffer->SetupFormat();
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void CGfxVertexArrayObject::Bind(void) const
{
	GLBindVertexArray(m_vao);
}
