#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3Texture2DManager::CGLES3Texture2DManager(void)
{

}

CGLES3Texture2DManager::~CGLES3Texture2DManager(void)
{
	for (const auto& itTexture : m_pTextures) {
		delete itTexture.second;
	}
}

CGLES3Texture2D* CGLES3Texture2DManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itTexture = m_pTextures.find(name);

		if (itTexture != m_pTextures.end()) {
			return itTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CGLES3Texture2D* CGLES3Texture2DManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
		}

		return m_pTextures[name];
	}
}

CGLES3Texture2D* CGLES3Texture2DManager::Create(const char* szFileName, int baseLevel, int numLevels)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
			ResourceLoader()->LoadTexture2D(szFileName, m_pTextures[name], baseLevel, numLevels);
		}

		return m_pTextures[name];
	}
}

void CGLES3Texture2DManager::Destroy(CGLES3Texture2D* pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures.find(pTexture->GetName()) != m_pTextures.end()) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


CGLES3Texture2DArrayManager::CGLES3Texture2DArrayManager(void)
{

}

CGLES3Texture2DArrayManager::~CGLES3Texture2DArrayManager(void)
{
	for (const auto& itTexture : m_pTextures) {
		delete itTexture.second;
	}
}

CGLES3Texture2DArray* CGLES3Texture2DArrayManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itTexture = m_pTextures.find(name);

		if (itTexture != m_pTextures.end()) {
			return itTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CGLES3Texture2DArray* CGLES3Texture2DArrayManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
		}

		return m_pTextures[name];
	}
}

CGLES3Texture2DArray* CGLES3Texture2DArrayManager::Create(const char* szFileName, int baseLevel, int numLevels)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
			ResourceLoader()->LoadTexture2DArray(szFileName, m_pTextures[name], baseLevel, numLevels);
		}

		return m_pTextures[name];
	}
}

void CGLES3Texture2DArrayManager::Destroy(CGLES3Texture2DArray* pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures.find(pTexture->GetName()) != m_pTextures.end()) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


CGLES3TextureCubemapManager::CGLES3TextureCubemapManager(void)
{

}

CGLES3TextureCubemapManager::~CGLES3TextureCubemapManager(void)
{
	for (const auto& itTexture : m_pTextures) {
		delete itTexture.second;
	}
}

CGLES3TextureCubemap* CGLES3TextureCubemapManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itTexture = m_pTextures.find(name);

		if (itTexture != m_pTextures.end()) {
			return itTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CGLES3TextureCubemap* CGLES3TextureCubemapManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubemap(this, name);
		}

		return m_pTextures[name];
	}
}

CGLES3TextureCubemap* CGLES3TextureCubemapManager::Create(const char* szFileName, int baseLevel, int numLevels)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubemap(this, name);
			ResourceLoader()->LoadTextureCubemap(szFileName, m_pTextures[name], baseLevel, numLevels);
		}

		return m_pTextures[name];
	}
}

void CGLES3TextureCubemapManager::Destroy(CGLES3TextureCubemap* pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures.find(pTexture->GetName()) != m_pTextures.end()) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
