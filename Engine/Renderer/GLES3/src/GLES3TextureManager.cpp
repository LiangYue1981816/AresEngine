#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3TextureManager::CGLES3TextureManager(void)
{

}

CGLES3TextureManager::~CGLES3TextureManager(void)
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

bool CGLES3TextureManager::IsHaveTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTexture2Ds.find(name) != m_pTexture2Ds.end() && m_pTexture2Ds[name]->GetType() == GFX_TEXTURE_2D;
	}
}

bool CGLES3TextureManager::IsHaveTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTexture2DArrays.find(name) != m_pTexture2DArrays.end() && m_pTexture2DArrays[name]->GetType() == GFX_TEXTURE_2D_ARRAY;
	}
}

bool CGLES3TextureManager::IsHaveTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextureCubeMaps.find(name) != m_pTextureCubeMaps.end() && m_pTextureCubeMaps[name]->GetType() == GFX_TEXTURE_CUBE_MAP;
	}
}

CGLES3Texture2D* CGLES3TextureManager::CreateTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2Ds[name] == nullptr) {
			m_pTexture2Ds[name] = new CGLES3Texture2D(this, name);
		}

		return (CGLES3Texture2D *)m_pTexture2Ds[name];
	}
}

CGLES3Texture2D* CGLES3TextureManager::CreateTexture2D(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2Ds[name] == nullptr) {
			m_pTexture2Ds[name] = new CGLES3Texture2D(this, name);
			ResourceLoader()->LoadTexture2D(szFileName, (CGLES3Texture2D *)m_pTexture2Ds[name]);
		}

		return (CGLES3Texture2D *)m_pTexture2Ds[name];
	}
}

CGLES3Texture2DArray* CGLES3TextureManager::CreateTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2DArrays[name] == nullptr) {
			m_pTexture2DArrays[name] = new CGLES3Texture2DArray(this, name);
		}

		return (CGLES3Texture2DArray *)m_pTexture2DArrays[name];
	}
}

CGLES3Texture2DArray* CGLES3TextureManager::CreateTexture2DArray(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTexture2DArrays[name] == nullptr) {
			m_pTexture2DArrays[name] = new CGLES3Texture2DArray(this, name);
			ResourceLoader()->LoadTexture2DArray(szFileName, (CGLES3Texture2DArray *)m_pTexture2DArrays[name]);
		}

		return (CGLES3Texture2DArray *)m_pTexture2DArrays[name];
	}
}

CGLES3TextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextureCubeMaps[name] == nullptr) {
			m_pTextureCubeMaps[name] = new CGLES3TextureCubeMap(this, name);
		}

		return (CGLES3TextureCubeMap *)m_pTextureCubeMaps[name];
	}
}

CGLES3TextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextureCubeMaps[name] == nullptr) {
			m_pTextureCubeMaps[name] = new CGLES3TextureCubeMap(this, name);
			ResourceLoader()->LoadTextureCubeMap(szFileName, (CGLES3TextureCubeMap *)m_pTextureCubeMaps[name]);
		}

		return (CGLES3TextureCubeMap *)m_pTextureCubeMaps[name];
	}
}

void CGLES3TextureManager::DestroyTexture2D(CGfxTexture2D *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTexture2Ds.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}

void CGLES3TextureManager::DestroyTexture2DArray(CGfxTexture2DArray *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTexture2DArrays.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}

void CGLES3TextureManager::DestroyTextureCubeMap(CGfxTextureCubeMap *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextureCubeMaps.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
