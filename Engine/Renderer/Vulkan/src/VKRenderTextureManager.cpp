#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKRenderTextureManager::CVKRenderTextureManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKRenderTextureManager::~CVKRenderTextureManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CVKDevice* CVKRenderTextureManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKRenderTextureManager::IsHaveRenderTexture(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CVKRenderTexture* CVKRenderTextureManager::CreateRenderTexture(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKRenderTexture(m_pDevice, this, name);
		}

		return (CVKRenderTexture *)m_pTextures[name];
	}
}

void CVKRenderTextureManager::Destroy(CGfxRenderTexture *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
