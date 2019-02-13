#include "GfxHeader.h"


CGfxUniformVec3::CGfxUniformVec3(void)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_value));
}

CGfxUniformVec3::~CGfxUniformVec3(void)
{

}

CGfxUniformBufferPtr CGfxUniformVec3::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CGfxUniformVec3::SetValue(float v0, float v1, float v2)
{
	m_bDirty = true;
	m_value = glm::vec3(v0, v1, v2);
}

void CGfxUniformVec3::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
