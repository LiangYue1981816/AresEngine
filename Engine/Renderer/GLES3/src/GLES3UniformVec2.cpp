#include "GLES3Renderer.h"
#include "GLES3UniformVec2.h"
#include "GLES3UniformBuffer.h"


CGLES3UniformVec2::CGLES3UniformVec2(bool bDynamic)
	: CGLES3UniformBase(nullptr, sizeof(m_value), bDynamic)
	, m_bDirty(false)
{

}

CGLES3UniformVec2::~CGLES3UniformVec2(void)
{

}

void CGLES3UniformVec2::Release(void)
{
	delete this;
}

void CGLES3UniformVec2::SetValue(float v0, float v1)
{
	m_bDirty = true;
	m_value = glm::vec2(v0, v1);
}

void CGLES3UniformVec2::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
