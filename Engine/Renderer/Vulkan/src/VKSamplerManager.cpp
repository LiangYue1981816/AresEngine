#include "VKRenderer.h"


CVKSamplerManager::CVKSamplerManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKSamplerManager::~CVKSamplerManager(void)
{
	for (const auto &itSampler : m_pSamplers) {
		delete itSampler.second;
	}

	m_pSamplers.clear();
}

CVKSampler* CVKSamplerManager::Create(int mipLevels, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	mutex_autolock autolock(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X_%8.8X_%8.8X_%8.8X_%8.8X", mipLevels, minFilter, magFilter, mipmapMode, addressMode);

		uint32_t name = HashValue(szName);

		if (m_pSamplers[name] == nullptr) {
			m_pSamplers[name] = new CVKSampler(m_pDevice, mipLevels, minFilter, magFilter, mipmapMode, addressMode);
		}

		return (CVKSampler *)m_pSamplers[name];
	}
}
