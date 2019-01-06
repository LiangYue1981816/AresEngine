#include "VKRenderer.h"


CVKShaderManager::CVKShaderManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKShaderManager::~CVKShaderManager(void)
{

}

CVKShader* CVKShaderManager::Create(const char *szFileName, shader_kind kind)
{
	return nullptr;
}
