#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformVec3.h"
#include "GfxUniformBuffer.h"


CGfxUniformVec3::CGfxUniformVec3(void)
	: m_bDirty(false)
	, m_pUniformBuffer(NULL)
{
	m_pUniformBuffer = new CGfxUniformBuffer;
	m_pUniformBuffer->Create(NULL, sizeof(m_value), true);
}

CGfxUniformVec3::~CGfxUniformVec3(void)
{
	m_pUniformBuffer->Destroy();
	delete m_pUniformBuffer;
}

void CGfxUniformVec3::SetValue(float x, float y, float z)
{
	m_bDirty = true;
	m_value = glm::vec3(x, y, z);
}

void CGfxUniformVec3::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->SetData(&m_value, sizeof(m_value));
	}
}

GLuint CGfxUniformVec3::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}

GLuint CGfxUniformVec3::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}
