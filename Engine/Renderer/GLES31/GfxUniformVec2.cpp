#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformVec2.h"
#include "GfxUniformBuffer.h"


CGfxUniformVec2::CGfxUniformVec2(void)
	: m_bDirty(false)
	, m_pUniformBuffer(NULL)
{
	m_pUniformBuffer = new CGfxUniformBuffer(sizeof(m_value), true);
}

CGfxUniformVec2::~CGfxUniformVec2(void)
{
	delete m_pUniformBuffer;
}

void CGfxUniformVec2::SetValue(float x, float y)
{
	m_bDirty = true;
	m_value = glm::vec2(x, y);
}

void CGfxUniformVec2::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}

GLuint CGfxUniformVec2::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}

GLsizeiptr CGfxUniformVec2::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}
