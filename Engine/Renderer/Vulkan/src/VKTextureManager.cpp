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
}

CVKTexture2D* CVKTexture2DManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto &itTexture = m_pTextures.find(name);

		if (itTexture != m_pTextures.end()) {
			return itTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKTexture2D* CVKTexture2DManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2D(m_pDevice, this, name);
		}

		return m_pTextures[name];
	}
}

CVKTexture2D* CVKTexture2DManager::Create(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2D(m_pDevice, this, name);
			ResourceLoader()->LoadTexture2D(szFileName, m_pTextures[name]);
		}

		return m_pTextures[name];
	}
}

void CVKTexture2DManager::Destroy(CVKTexture2D *pTexture)
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
}

CVKTexture2DArray* CVKTexture2DArrayManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto &itTexture = m_pTextures.find(name);

		if (itTexture != m_pTextures.end()) {
			return itTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKTexture2DArray* CVKTexture2DArrayManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2DArray(m_pDevice, this, name);
		}

		return m_pTextures[name];
	}
}

CVKTexture2DArray* CVKTexture2DArrayManager::Create(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2DArray(m_pDevice, this, name);
			ResourceLoader()->LoadTexture2DArray(szFileName, m_pTextures[name]);
		}

		return (CVKTexture2DArray *)m_pTextures[name];
	}
}

void CVKTexture2DArrayManager::Destroy(CVKTexture2DArray *pTexture)
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
}

CVKTextureCubeMap* CVKTextureCubeMapManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto &itTexture = m_pTextures.find(name);

		if (itTexture != m_pTextures.end()) {
			return itTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKTextureCubeMap* CVKTextureCubeMapManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTextureCubeMap(m_pDevice, this, name);
		}

		return m_pTextures[name];
	}
}

CVKTextureCubeMap* CVKTextureCubeMapManager::Create(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTextureCubeMap(m_pDevice, this, name);
			ResourceLoader()->LoadTextureCubeMap(szFileName, m_pTextures[name]);
		}

		return m_pTextures[name];
	}
}

void CVKTextureCubeMapManager::Destroy(CVKTextureCubeMap *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
