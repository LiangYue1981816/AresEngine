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

CGfxShader* CGfxShaderManager::CreateShader(uint32_t name)
{
	if (m_pShaders[name] == NULL) {
		m_pShaders[name] = new CGfxShader(name);
	}

	return m_pShaders[name];
}
