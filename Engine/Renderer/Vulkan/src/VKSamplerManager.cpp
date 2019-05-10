#include "VKRenderer.h"


CVKSamplerManager::CVKSamplerManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{
	ASSERT(m_pDevice);
}

CVKSamplerManager::~CVKSamplerManager(void)
{
	for (const auto& itSampler : m_pSamplers) {
		delete itSampler.second;
	}
}

CVKSampler* CVKSamplerManager::Create(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%x_%x_%x_%x", minFilter, magFilter, mipmapMode, addressMode);

		if (m_pSamplers[name] == nullptr) {
			m_pSamplers[name] = new CVKSampler(m_pDevice, minFilter, magFilter, mipmapMode, addressMode);
		}

		return m_pSamplers[name];
	}
}
