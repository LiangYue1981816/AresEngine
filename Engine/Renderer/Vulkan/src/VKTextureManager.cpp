#include "VKRenderer.h"


CVKTexture2DManager::CVKTexture2DManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKTexture2DManager::~CVKTexture2DManager(void)
{
	for (const auto& itTexture : m_pTextures) {
		delete itTexture.second;
	}
}

CVKTexture2D* CVKTexture2DManager::Get(uint32_t name)
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

CVKTexture2D* CVKTexture2DManager::Create(uint32_t name, GfxPixelFormat format, int width, int height, int levels, int samples)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2D(m_pDevice, this, name, format, width, height, levels, samples);
		}

		return m_pTextures[name];
	}
}

void CVKTexture2DManager::Destroy(CVKTexture2D* pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures.find(pTexture->GetName()) != m_pTextures.end()) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


CVKTexture2DArrayManager::CVKTexture2DArrayManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKTexture2DArrayManager::~CVKTexture2DArrayManager(void)
{
	for (const auto& itTexture : m_pTextures) {
		delete itTexture.second;
	}
}

CVKTexture2DArray* CVKTexture2DArrayManager::Get(uint32_t name)
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

CVKTexture2DArray* CVKTexture2DArrayManager::Create(uint32_t name, GfxPixelFormat format, int width, int height, int layers, int levels)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTexture2DArray(m_pDevice, this, name, format, width, height, layers, levels);
		}

		return m_pTextures[name];
	}
}

void CVKTexture2DArrayManager::Destroy(CVKTexture2DArray* pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures.find(pTexture->GetName()) != m_pTextures.end()) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


CVKTextureCubemapManager::CVKTextureCubemapManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKTextureCubemapManager::~CVKTextureCubemapManager(void)
{
	for (const auto& itTexture : m_pTextures) {
		delete itTexture.second;
	}
}

CVKTextureCubemap* CVKTextureCubemapManager::Get(uint32_t name)
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

CVKTextureCubemap* CVKTextureCubemapManager::Create(uint32_t name, GfxPixelFormat format, int width, int height, int levels)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CVKTextureCubemap(m_pDevice, this, name, format, width, height, levels);
		}

		return m_pTextures[name];
	}
}

void CVKTextureCubemapManager::Destroy(CVKTextureCubemap* pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures.find(pTexture->GetName()) != m_pTextures.end()) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
