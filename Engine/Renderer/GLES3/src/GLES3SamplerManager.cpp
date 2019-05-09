#include "GLES3Renderer.h"


CGLES3SamplerManager::CGLES3SamplerManager(void)
{

}

CGLES3SamplerManager::~CGLES3SamplerManager(void)
{
	for (const auto& itSampler : m_pSamplers) {
		delete itSampler.second;
	}
}

CGLES3Sampler* CGLES3SamplerManager::Create(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%x_%x_%x_%x", minFilter, magFilter, mipmapMode, addressMode);

		if (m_pSamplers[name] == nullptr) {
			m_pSamplers[name] = new CGLES3Sampler(minFilter, magFilter, mipmapMode, addressMode);
		}

		return m_pSamplers[name];
	}
}
