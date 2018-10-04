#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxShaderManager.h"


CGfxShaderManager::CGfxShaderManager(void)
{

}

CGfxShaderManager::~CGfxShaderManager(void)
{
	for (const auto &itShader : m_pShaders) {
		delete itShader.second;
	}

	m_pShaders.clear();
}

CGfxShader* CGfxShaderManager::LoadShader(const char *szFileName, shaderc_shader_kind kind)
{
	uint32_t name = HashValue(szFileName);

	if (m_pShaders[name] == NULL) {
		m_pShaders[name] = new CGfxShader(name);
		m_pShaders[name]->Load(szFileName, kind);
	}

	return m_pShaders[name];
}
