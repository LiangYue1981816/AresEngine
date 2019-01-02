#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKTextureManager::CVKTextureManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKTextureManager::~CVKTextureManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CVKDevice* CVKTextureManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKTextureManager::IsHaveTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end() && m_pTextures[name]->GetType() == GFX_TEXTURE_2D;
	}
}

bool CVKTextureManager::IsHaveTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end() && m_pTextures[name]->GetType() == GFX_TEXTURE_2D_ARRAY;
	}
}

bool CVKTextureManager::IsHaveTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end() && m_pTextures[name]->GetType() == GFX_TEXTURE_CUBE_MAP;
	}
}

CVKTexture2D* CVKTextureManager::CreateTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2D(m_pDevice, this, name);
		}

		return (CVKTexture2D *)m_pTextures[name];
	}
}

CVKTexture2D* CVKTextureManager::CreateTexture2D(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2D(m_pDevice, this, name);
			ResourceLoader()->LoadTexture2D(szFileName, (CVKTexture2D *)m_pTextures[name]);
		}

		return (CVKTexture2D *)m_pTextures[name];
	}
}

CVKTexture2DArray* CVKTextureManager::CreateTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2DArray(m_pDevice, this, name);
		}

		return (CVKTexture2DArray *)m_pTextures[name];
	}
}

CVKTexture2DArray* CVKTextureManager::CreateTexture2DArray(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2DArray(m_pDevice, this, name);
			ResourceLoader()->LoadTexture2DArray(szFileName, (CVKTexture2DArray *)m_pTextures[name]);
		}

		return (CVKTexture2DArray *)m_pTextures[name];
	}
}

CVKTextureCubeMap* CVKTextureManager::CreateTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTextureCubeMap(m_pDevice, this, name);
		}

		return (CVKTextureCubeMap *)m_pTextures[name];
	}
}

CVKTextureCubeMap* CVKTextureManager::CreateTextureCubeMap(const char *szFileName)
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

void CVKTextureManager::Destroy(CGfxTexture *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
