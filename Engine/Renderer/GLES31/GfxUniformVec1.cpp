#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformVec1.h"
#include "GfxUniformBuffer.h"


CGfxUniformVec1::CGfxUniformVec1(void)
	: m_bDirty(false)
	, m_pUniformBuffer(nullptr)
{
	m_pUniformBuffer = new CGfxUniformBuffer(sizeof(m_value), true);
}

CGfxUniformVec1::~CGfxUniformVec1(void)
{
	delete m_pUniformBuffer;
}

void CGfxUniformVec1::SetValue(float value)
{
	m_bDirty = true;
	m_value = value;
}

void CGfxUniformVec1::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_pUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}

uint32_t CGfxUniformVec1::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}

uint32_t CGfxUniformVec1::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}
