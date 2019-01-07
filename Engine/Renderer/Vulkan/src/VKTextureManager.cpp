#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKTextureManager::CVKTextureManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKTextureManager::~CVKTextureManager(void)
{
	for (const auto &itTexture : m_pTexture2Ds) {
		delete itTexture.second;
	}

	for (const auto &itTexture : m_pTexture2DArrays) {
		delete itTexture.second;
	}

	for (const auto &itTexture : m_pTextureCubeMaps) {
		delete itTexture.second;
	}

	m_pTexture2Ds.clear();
	m_pTexture2DArrays.clear();
	m_pTextureCubeMaps.clear();
}

CVKDevice* CVKTextureManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKTextureManager::IsHaveTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTexture2Ds.find(name) != m_pTexture2Ds.end() && m_pTexture2Ds[name]->GetType() == GFX_TEXTURE_2D;
	}
}

bool CVKTextureManager::IsHaveTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTexture2DArrays.find(name) != m_pTexture2DArrays.end() && m_pTexture2DArrays[name]->GetType() == GFX_TEXTURE_2D_ARRAY;
	}
}

bool CVKTextureManager::IsHaveTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextureCubeMaps.find(name) != m_pTextureCubeMaps.end() && m_pTextureCubeMaps[name]->GetType() == GFX_TEXTURE_CUBE_MAP;
	}
}

CVKTexture2D* CVKTextureManager::CreateTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2Ds[name] == nullptr) {
			m_pTexture2Ds[name] = new CVKTexture2D(m_pDevice, this, name);
		}

		return (CVKTexture2D *)m_pTexture2Ds[name];
	}
}

CVKTexture2D* CVKTextureManager::CreateTexture2D(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2Ds[name] == nullptr) {
			m_pTexture2Ds[name] = new CVKTexture2D(m_pDevice, this, name);
			ResourceLoader()->LoadTexture2D(szFileName, (CVKTexture2D *)m_pTexture2Ds[name]);
		}

		return (CVKTexture2D *)m_pTexture2Ds[name];
	}
}

CVKTexture2DArray* CVKTextureManager::CreateTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2DArrays[name] == nullptr) {
			m_pTexture2DArrays[name] = new CVKTexture2DArray(m_pDevice, this, name);
		}

		return (CVKTexture2DArray *)m_pTexture2DArrays[name];
	}
}

CVKTexture2DArray* CVKTextureManager::CreateTexture2DArray(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2DArrays[name] == nullptr) {
			m_pTexture2DArrays[name] = new CVKTexture2DArray(m_pDevice, this, name);
			ResourceLoader()->LoadTexture2DArray(szFileName, (CVKTexture2DArray *)m_pTexture2DArrays[name]);
		}

		return (CVKTexture2DArray *)m_pTexture2DArrays[name];
	}
}

CVKTextureCubeMap* CVKTextureManager::CreateTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextureCubeMaps[name] == nullptr) {
			m_pTextureCubeMaps[name] = new CVKTextureCubeMap(m_pDevice, this, name);
		}

		return (CVKTextureCubeMap *)m_pTextureCubeMaps[name];
	}
}

CVKTextureCubeMap* CVKTextureManager::CreateTextureCubeMap(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextureCubeMaps[name] == nullptr) {
			m_pTextureCubeMaps[name] = new CVKTextureCubeMap(m_pDevice, this, name);
			ResourceLoader()->LoadTextureCubeMap(szFileName, (CVKTextureCubeMap *)m_pTextureCubeMaps[name]);
		}

		return (CVKTextureCubeMap *)m_pTextureCubeMaps[name];
	}
}

void CVKTextureManager::DestroyTexture2D(CGfxTexture2D *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTexture2Ds.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}

void CVKTextureManager::DestroyTexture2DArray(CGfxTexture2DArray *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTexture2DArrays.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}

void CVKTextureManager::DestroyTextureCubeMap(CGfxTextureCubeMap *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextureCubeMaps.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
