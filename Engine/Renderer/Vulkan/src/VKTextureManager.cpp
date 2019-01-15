#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKTexture2DManager::CVKTexture2DManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKTexture2DManager::~CVKTexture2DManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CVKDevice* CVKTexture2DManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKTexture2DManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CVKTexture2D* CVKTexture2DManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2D(m_pDevice, this, name);
		}

		return (CVKTexture2D *)m_pTextures[name];
	}
}

CVKTexture2D* CVKTexture2DManager::Create(const char *szFileName)
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

void CVKTexture2DManager::Destroy(CGfxTexture2D *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


CVKTexture2DArrayManager::CVKTexture2DArrayManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKTexture2DArrayManager::~CVKTexture2DArrayManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CVKDevice* CVKTexture2DArrayManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKTexture2DArrayManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CVKTexture2DArray* CVKTexture2DArrayManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2DArray(m_pDevice, this, name);
		}

		return (CVKTexture2DArray *)m_pTextures[name];
	}
}

CVKTexture2DArray* CVKTexture2DArrayManager::Create(const char *szFileName)
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

void CVKTexture2DArrayManager::Destroy(CGfxTexture2DArray *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


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
