#include "GLES3Renderer.h"
#include "GLES3UniformVec3.h"
#include "GLES3UniformBuffer.h"


CGLES3UniformVec3::CGLES3UniformVec3(bool bDynamic)
	: CGLES3UniformBase(nullptr, sizeof(m_value), bDynamic)
	, m_bDirty(false)
{

}

CGLES3UniformVec3::~CGLES3UniformVec3(void)
{

}

void CGLES3UniformVec3::Release(void)
{
	delete this;
}

void CGLES3UniformVec3::SetValue(float v0, float v1, float v2)
{
	m_bDirty = true;
	m_value = glm::vec3(v0, v1, v2);
}

void CGLES3UniformVec3::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
