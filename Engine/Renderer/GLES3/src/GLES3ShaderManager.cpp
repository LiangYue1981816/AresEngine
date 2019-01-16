#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3ShaderManager::CGLES3ShaderManager(void)
{

}

CGLES3ShaderManager::~CGLES3ShaderManager(void)
{
	for (const auto &itShader : m_pShaders) {
		delete itShader.second;
	}

	m_pShaders.clear();
}

CGLES3Shader* CGLES3ShaderManager::Create(const char *szFileName, shader_kind kind)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pShaders[name] == nullptr) {
			m_pShaders[name] = new CGLES3Shader(name);
			ResourceLoader()->LoadShader(szFileName, m_pShaders[name], kind);
		}

		return m_pShaders[name];
	}
}
