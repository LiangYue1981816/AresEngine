#include "GLES3Renderer.h"


CGLES3VertexArrayObject::CGLES3VertexArrayObject(void)
	: m_vao(0)
{

}

CGLES3VertexArrayObject::CGLES3VertexArrayObject(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer)
	: m_vao(0)
{
	Create(pIndexBuffer, pVertexBuffer, pInstanceBuffer);
}

CGLES3VertexArrayObject::~CGLES3VertexArrayObject(void)
{
	Destroy();
}

void CGLES3VertexArrayObject::Release(void)
{
	delete this;
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


CGLES3MultiVertexArrayObject::CGLES3MultiVertexArrayObject(int count)
	: m_index(0)
	, m_pVertexArrayObjects(std::max(1, count))
{
	for (int index = 0; index < m_pVertexArrayObjects.size(); index++) {
		m_pVertexArrayObjects[index] = new CGLES3VertexArrayObject;
	}
}

CGLES3MultiVertexArrayObject::~CGLES3MultiVertexArrayObject(void)
{
	for (auto& itVertexArrayObjects : m_pVertexArrayObjects) {
		delete itVertexArrayObjects;
	}
}

void CGLES3MultiVertexArrayObject::Release(void)
{
	delete this;
}

bool CGLES3MultiVertexArrayObject::SetIndex(int index)
{
	if (index >= 0 && index < m_pVertexArrayObjects.size()) {
		m_index = index;
		return true;
	}
	else {
		return false;
	}
}

CGLES3VertexArrayObject* CGLES3MultiVertexArrayObject::GetVertexArrayObject(void) const
{
	return m_pVertexArrayObjects[m_index];
}

CGLES3VertexArrayObject* CGLES3MultiVertexArrayObject::GetVertexArrayObject(int index) const
{
	if (index >= 0 && index < m_pVertexArrayObjects.size()) {
		return m_pVertexArrayObjects[index];
	}
	else {
		return nullptr;
	}
}

bool CGLES3MultiVertexArrayObject::Create(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer)
{
	return m_pVertexArrayObjects[m_index]->Create(pIndexBuffer, pVertexBuffer, pInstanceBuffer);
}

void CGLES3MultiVertexArrayObject::Destroy(void)
{
	m_pVertexArrayObjects[m_index]->Destroy();
}

void CGLES3MultiVertexArrayObject::Bind(void) const
{
	m_pVertexArrayObjects[m_index]->Bind();
}
