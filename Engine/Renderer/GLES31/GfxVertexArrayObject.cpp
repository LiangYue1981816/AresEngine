#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxVertexArrayObject.h"


CGfxVertexArrayObject::CGfxVertexArrayObject(void)
	: m_vao(0)
	, m_pIndexBuffer(NULL)
	, m_pVertexBuffer(NULL)
	, m_pInstanceBuffer(NULL)
{
	glGenVertexArrays(1, &m_vao);
}

CGfxVertexArrayObject::~CGfxVertexArrayObject(void)
{
	glDeleteVertexArrays(1, &m_vao);
}

bool CGfxVertexArrayObject::Buffer(CGfxIndexBuffer *pIndexBuffer, CGfxVertexBuffer *pVertexBuffer, CGfxInstanceBuffer *pInstanceBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
	m_pVertexBuffer = pVertexBuffer;
	m_pInstanceBuffer = pInstanceBuffer;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(m_vao);
	{
		if (m_pIndexBuffer) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer->GetIndexBuffer());
		}

		if (m_pVertexBuffer) {
			glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer->GetVertexBuffer());
			m_pVertexBuffer->SetupFormat();
		}

		if (m_pInstanceBuffer) {
			glBindBuffer(GL_ARRAY_BUFFER, m_pInstanceBuffer->GetInstanceBuffer());
			m_pInstanceBuffer->SetupFormat();
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
