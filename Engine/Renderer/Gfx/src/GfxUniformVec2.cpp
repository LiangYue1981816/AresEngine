#include "GfxHeader.h"


CGfxUniformVec2::CGfxUniformVec2(bool bDynamic)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = Renderer()->NewUniformBuffer(sizeof(m_value), bDynamic);
}

CGfxUniformVec2::~CGfxUniformVec2(void)
{

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

const CGfxUniformBufferPtr& CGfxUniformVec2::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}
