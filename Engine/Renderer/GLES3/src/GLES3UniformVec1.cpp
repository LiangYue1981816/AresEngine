#include "GLES3Renderer.h"
#include "GLES3UniformVec1.h"
#include "GLES3UniformBuffer.h"


CGLES3UniformVec1::CGLES3UniformVec1(bool bDynamic)
	: CGLES3UniformBase(nullptr, sizeof(m_value), bDynamic)
	, m_bDirty(false)
{

}

CGLES3UniformVec1::~CGLES3UniformVec1(void)
{

}

void CGLES3UniformVec1::Release(void)
{
	delete this;
}

void CGLES3UniformVec1::SetValue(float v0)
{
	m_bDirty = true;
	m_value = v0;
}

void CGLES3UniformVec1::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
