#include "VKRenderer.h"


CVKShader::CVKShader(CVKDevice *pDevice, uint32_t name)
	: CGfxShader(name)
	, m_pDevice(pDevice)
{

}

CVKShader::~CVKShader(void)
{

}

bool CVKShader::Create(const uint32_t *words, size_t numWords, shader_kind kind)
{
	return true;
}

void CVKShader::Destroy(void)
{

}

bool CVKShader::IsValid(void) const
{
	return true;
}

uint32_t CVKShader::GetKind(void) const
{
	return 0;
}
