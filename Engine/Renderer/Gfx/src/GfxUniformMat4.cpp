#include "GfxHeader.h"


CGfxUniformMat4::CGfxUniformMat4(void)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_value));
}

CGfxUniformMat4::~CGfxUniformMat4(void)
{

}

const CGfxUniformBufferPtr CGfxUniformMat4::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CGfxUniformMat4::SetValue(const float *value)
{
	m_bDirty = true;
	m_value[0][0] = value[0];
	m_value[0][1] = value[1];
	m_value[0][2] = value[2];
	m_value[0][3] = value[3];
	m_value[1][0] = value[4];
	m_value[1][1] = value[5];
	m_value[1][2] = value[6];
	m_value[1][3] = value[7];
	m_value[2][0] = value[8];
	m_value[2][1] = value[9];
	m_value[2][2] = value[10];
	m_value[2][3] = value[11];
	m_value[3][0] = value[12];
	m_value[3][1] = value[13];
	m_value[3][2] = value[14];
	m_value[3][3] = value[15];
}

void CGfxUniformMat4::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_value), &m_value);
	}
}
