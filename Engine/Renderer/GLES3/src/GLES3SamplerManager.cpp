#include "GLES3Renderer.h"


CGLES3SamplerManager::CGLES3SamplerManager(void)
{

}

CGLES3SamplerManager::~CGLES3SamplerManager(void)
{
	for (const auto &itSampler : m_pSamplers) {
		delete itSampler.second;
	}

	m_pSamplers.clear();
}

CGLES3Sampler* CGLES3SamplerManager::Create(int mipLevels, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%x_%x_%x_%x_%x", mipLevels, minFilter, magFilter, mipmapMode, addressMode);

		if (m_pSamplers[name] == nullptr) {
			m_pSamplers[name] = new CGLES3Sampler(mipLevels, minFilter, magFilter, mipmapMode, addressMode);
		}

		return m_pSamplers[name];
	}
}
