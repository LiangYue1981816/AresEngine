#include "GLES3Renderer.h"
#include "GLES3ShaderManager.h"


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

CGLES3Shader* CGLES3ShaderManager::LoadShader(const char *szFileName, shaderc_shader_kind kind)
{
	uint32_t name = HashValue(szFileName);

	if (m_pShaders[name] == nullptr) {
		m_pShaders[name] = new CGLES3Shader(name);
		m_pShaders[name]->Load(szFileName, kind);
	}

	return m_pShaders[name];
}
