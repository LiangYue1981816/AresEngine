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
	GLuint64 name = (((GLuint64)minFilter) << 32) | (((GLuint64)magFilter) << 16) | ((GLuint64)addressMode);

	if (m_pSamplers[name] == NULL) {
		m_pSamplers[name] = new CGfxSampler(minFilter, magFilter, addressMode);
	}

	return m_pSamplers[name];
}
