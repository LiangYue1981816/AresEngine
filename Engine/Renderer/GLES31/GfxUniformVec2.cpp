#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformVec2.h"
#include "GfxUniformBuffer.h"


CGfxUniformVec2::CGfxUniformVec2(void)
	: m_bDirty(false)
	, m_pUniformBuffer(NULL)
{
	m_pUniformBuffer = new CGfxUniformBuffer;
	m_pUniformBuffer->Create(NULL, sizeof(m_value), true);
}

CGfxUniformVec2::~CGfxUniformVec2(void)
{
	m_pUniformBuffer->Destroy();
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
		m_pUniformBuffer->SetData(&m_value, sizeof(m_value));
	}
}

GLuint CGfxUniformVec2::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}

GLuint CGfxUniformVec2::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}
