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
	uint64_t name = (((uint64_t)minFilter) << 32) | (((uint64_t)magFilter) << 16) | ((uint64_t)addressMode);

	if (m_pSamplers[name] == NULL) {
		m_pSamplers[name] = new CGfxSampler(minFilter, magFilter, addressMode);
	}

	return m_pSamplers[name];
}
