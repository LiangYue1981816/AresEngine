#include "GfxHeader.h"


CGLES3UniformVec4::CGLES3UniformVec4(bool bDynamic)
	: CGLES3UniformBase(nullptr, sizeof(m_value), bDynamic)
	, m_bDirty(false)
{

}

CGLES3UniformVec4::~CGLES3UniformVec4(void)
{

}

void CGLES3UniformVec4::Release(void)
{
	delete this;
}

void CGLES3UniformVec4::SetValue(float v0, float v1, float v2, float v3)
{
	m_bDirty = true;
	m_value = glm::vec4(v0, v1, v2, v3);
}

void CGLES3UniformVec4::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
