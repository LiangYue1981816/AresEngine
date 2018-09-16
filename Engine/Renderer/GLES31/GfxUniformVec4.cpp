#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformVec4.h"
#include "GfxUniformBuffer.h"


CGfxUniformVec4::CGfxUniformVec4(void)
	: m_bDirty(false)
	, m_pUniformBuffer(NULL)
{
	m_pUniformBuffer = new CGfxUniformBuffer;
	m_pUniformBuffer->Create(NULL, sizeof(m_value), true);
}

CGfxUniformVec4::~CGfxUniformVec4(void)
{
	m_pUniformBuffer->Destroy();
	delete m_pUniformBuffer;
}

void CGfxUniformVec4::SetValue(float x, float y, float z, float w)
{
	m_bDirty = true;
	m_value = glm::vec4(x, y, z, w);
}

void CGfxUniformVec4::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->SetData(&m_value, sizeof(m_value));
	}
}

GLuint CGfxUniformVec4::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}

GLuint CGfxUniformVec4::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}
