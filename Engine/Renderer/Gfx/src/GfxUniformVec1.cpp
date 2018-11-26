#include "GfxHeader.h"


CGfxUniformVec1::CGfxUniformVec1(bool bDynamic)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = Renderer()->NewUniformBuffer(sizeof(m_value), bDynamic);
}

CGfxUniformVec1::~CGfxUniformVec1(void)
{

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

const CGfxUniformBufferPtr& CGfxUniformVec1::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}
