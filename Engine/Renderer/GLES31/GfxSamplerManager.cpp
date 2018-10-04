#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxSamplerManager.h"


CGfxSamplerManager::CGfxSamplerManager(void)
{

}

CGfxSamplerManager::~CGfxSamplerManager(void)
{
	for (const auto &itSampler : m_pSamplers) {
		delete itSampler.second;
	}

	m_pSamplers.clear();
}

CGfxSampler* CGfxSamplerManager::CreateSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode)
{
	char szName[_MAX_STRING];
	sprintf(szName, "%8.8X_%8.8X_%8.8X", minFilter, magFilter, addressMode);

	uint32_t name = HashValue(szName);

	if (m_pSamplers[name] == NULL) {
		m_pSamplers[name] = new CGfxSampler(minFilter, magFilter, addressMode);
	}

	return m_pSamplers[name];
}
