#include "VKRenderer.h"


CVKRenderTextureManager::CVKRenderTextureManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKRenderTextureManager::~CVKRenderTextureManager(void)
{
	for (const auto& itRenderTexture : m_pRenderTextures) {
		delete itRenderTexture.second;
	}
}

CVKRenderTexture* CVKRenderTextureManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itRenderTexture = m_pRenderTextures.find(name);

		if (itRenderTexture != m_pRenderTextures.end()) {
			return itRenderTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKRenderTexture* CVKRenderTextureManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pRenderTextures[name] == nullptr) {
			m_pRenderTextures[name] = new CVKRenderTexture(m_pDevice, this, name);
		}

		return m_pRenderTextures[name];
	}
}

void CVKRenderTextureManager::Destroy(CVKRenderTexture* pRenderTexture)
{
	ASSERT(pRenderTexture);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pRenderTextures.find(pRenderTexture->GetName()) != m_pRenderTextures.end()) {
				m_pRenderTextures.erase(pRenderTexture->GetName());
			}
		}
	}
	delete pRenderTexture;
}
