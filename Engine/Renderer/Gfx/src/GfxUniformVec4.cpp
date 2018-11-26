#include "GfxHeader.h"


CGfxUniformVec4::CGfxUniformVec4(bool bDynamic)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = Renderer()->NewUniformBuffer(sizeof(m_value), bDynamic);
}

CGfxUniformVec4::~CGfxUniformVec4(void)
{

}

void CGfxUniformVec4::SetValue(float v0, float v1, float v2, float v3)
{
	m_bDirty = true;
	m_value = glm::vec4(v0, v1, v2, v3);
}

void CGfxUniformVec4::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}

const CGfxUniformBufferPtr& CGfxUniformVec4::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}
