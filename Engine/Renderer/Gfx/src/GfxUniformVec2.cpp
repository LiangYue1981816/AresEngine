#include "GfxHeader.h"


CGfxUniformVec2::CGfxUniformVec2(void)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_value));
}

CGfxUniformVec2::~CGfxUniformVec2(void)
{

}

const glm::vec2& CGfxUniformVec2::GetValue(void) const
{
	return m_value;
}

const CGfxUniformBufferPtr CGfxUniformVec2::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CGfxUniformVec2::SetValue(float v0, float v1)
{
	m_bDirty = true;
	m_value = glm::vec2(v0, v1);
}

void CGfxUniformVec2::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
