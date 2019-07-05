#include "GfxHeader.h"


CGfxUniformVec1::CGfxUniformVec1(void)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_value));
}

CGfxUniformVec1::~CGfxUniformVec1(void)
{

}

const float CGfxUniformVec1::GetValue(void) const
{
	return m_value;
}

const CGfxUniformBufferPtr CGfxUniformVec1::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CGfxUniformVec1::SetValue(float v0)
{
	m_bDirty = true;
	m_value = v0;
}

void CGfxUniformVec1::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
