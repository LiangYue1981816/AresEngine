#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKTextureCubeMapManager::CVKTextureCubeMapManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKTextureCubeMapManager::~CVKTextureCubeMapManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CVKDevice* CVKTextureCubeMapManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKTextureCubeMapManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CVKTextureCubeMap* CVKTextureCubeMapManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTextureCubeMap(m_pDevice, this, name);
		}

		return (CVKTextureCubeMap *)m_pTextures[name];
	}
}

CVKTextureCubeMap* CVKTextureCubeMapManager::Create(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTextureCubeMap(m_pDevice, this, name);
			ResourceLoader()->LoadTextureCubeMap(szFileName, (CVKTextureCubeMap *)m_pTextures[name]);
		}

		return (CVKTextureCubeMap *)m_pTextures[name];
	}
}

void CVKTextureCubeMapManager::Destroy(CGfxTextureCubeMap *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
