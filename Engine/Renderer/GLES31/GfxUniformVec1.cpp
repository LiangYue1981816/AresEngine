#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformVec1.h"
#include "GfxUniformBuffer.h"


CGfxUniformVec1::CGfxUniformVec1(void)
	: m_bDirty(false)
	, m_pUniformBuffer(NULL)
{
	m_pUniformBuffer = new CGfxUniformBuffer;
	m_pUniformBuffer->Create(NULL, sizeof(m_value), true);
}

CGfxUniformVec1::~CGfxUniformVec1(void)
{
	m_pUniformBuffer->Destroy();
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
		m_pUniformBuffer->SetData(&m_value, sizeof(m_value));
	}
}

GLuint CGfxUniformVec1::GetSize(void) const
{
	return m_pUniformBuffer->GetSize();
}

GLuint CGfxUniformVec1::GetBuffer(void) const
{
	return m_pUniformBuffer->GetBuffer();
}
