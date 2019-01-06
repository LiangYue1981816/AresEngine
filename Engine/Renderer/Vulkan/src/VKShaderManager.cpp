#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKShaderManager::CVKShaderManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKShaderManager::~CVKShaderManager(void)
{
	for (const auto &itShader : m_pShaders) {
		delete itShader.second;
	}

	m_pShaders.clear();
}

CVKShader* CVKShaderManager::Create(const char *szFileName, shader_kind kind)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pShaders[name] == nullptr) {
			m_pShaders[name] = new CVKShader(m_pDevice, name);
			ResourceLoader()->LoadShader(szFileName, m_pShaders[name], kind);
		}

		return (CVKShader *)m_pShaders[name];
	}
}
