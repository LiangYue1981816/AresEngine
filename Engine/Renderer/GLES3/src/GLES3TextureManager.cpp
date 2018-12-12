#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3TextureManager::CGLES3TextureManager(void)
{

}

CGLES3TextureManager::~CGLES3TextureManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

bool CGLES3TextureManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CGLES3Texture2D* CGLES3TextureManager::CreateTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
		}

		return (CGLES3Texture2D *)m_pTextures[name];
	}
}

CGLES3Texture2D* CGLES3TextureManager::CreateTexture2D(uint32_t name, const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
		}

		ResourceLoader()->LoadTexture2D(szFileName, (CGLES3Texture2D *)m_pTextures[name]);
		return (CGLES3Texture2D *)m_pTextures[name];
	}
}

CGLES3Texture2D* CGLES3TextureManager::CreateTexture2D(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
			ResourceLoader()->LoadTexture2D(szFileName, (CGLES3Texture2D *)m_pTextures[name]);
		}

		return (CGLES3Texture2D *)m_pTextures[name];
	}
}

CGLES3Texture2DArray* CGLES3TextureManager::CreateTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
		}

		return (CGLES3Texture2DArray *)m_pTextures[name];
	}
}

CGLES3Texture2DArray* CGLES3TextureManager::CreateTexture2DArray(uint32_t name, const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
		}

		ResourceLoader()->LoadTexture2DArray(szFileName, (CGLES3Texture2DArray *)m_pTextures[name]);
		return (CGLES3Texture2DArray *)m_pTextures[name];
	}
}

CGLES3Texture2DArray* CGLES3TextureManager::CreateTexture2DArray(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
			ResourceLoader()->LoadTexture2DArray(szFileName, (CGLES3Texture2DArray *)m_pTextures[name]);
		}

		return (CGLES3Texture2DArray *)m_pTextures[name];
	}
}

CGLES3TextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubeMap(this, name);
		}

		return (CGLES3TextureCubeMap *)m_pTextures[name];
	}
}

CGLES3TextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(uint32_t name, const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubeMap(this, name);
		}

		ResourceLoader()->LoadTextureCubeMap(szFileName, (CGLES3TextureCubeMap *)m_pTextures[name]);
		return (CGLES3TextureCubeMap *)m_pTextures[name];
	}
}

CGLES3TextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubeMap(this, name);
			ResourceLoader()->LoadTextureCubeMap(szFileName, (CGLES3TextureCubeMap *)m_pTextures[name]);
		}

		return (CGLES3TextureCubeMap *)m_pTextures[name];
	}
}

void CGLES3TextureManager::Destroy(CGfxTextureBase *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
