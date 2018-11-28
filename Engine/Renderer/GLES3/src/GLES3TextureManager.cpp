#include "GfxHeader.h"


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

CGfxTexture2D* CGLES3TextureManager::CreateTexture2D(const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
		}

		return (CGfxTexture2D *)m_pTextures[name];
	}
}

CGfxTexture2DArray* CGLES3TextureManager::CreateTexture2DArray(const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
		}

		return (CGfxTexture2DArray *)m_pTextures[name];
	}
}

CGfxTextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubeMap(this, name);
		}

		return (CGfxTextureCubeMap *)m_pTextures[name];
	}
}

CGfxTexture2D* CGLES3TextureManager::CreateTexture2D(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
		}

		return (CGfxTexture2D *)m_pTextures[name];
	}
}

CGfxTexture2DArray* CGLES3TextureManager::CreateTexture2DArray(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
		}

		return (CGfxTexture2DArray *)m_pTextures[name];
	}
}

CGfxTextureCubeMap* CGLES3TextureManager::CreateTextureCubeMap(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubeMap(this, name);
		}

		return (CGfxTextureCubeMap *)m_pTextures[name];
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
