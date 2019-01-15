#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3Texture2DManager::CGLES3Texture2DManager(void)
{

}

CGLES3Texture2DManager::~CGLES3Texture2DManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

bool CGLES3Texture2DManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CGLES3Texture2D* CGLES3Texture2DManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2D(this, name);
		}

		return (CGLES3Texture2D *)m_pTextures[name];
	}
}

CGLES3Texture2D* CGLES3Texture2DManager::Create(const char *szFileName)
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

void CGLES3Texture2DManager::Destroy(CGfxTexture2D *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
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
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

bool CGLES3Texture2DArrayManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CGLES3Texture2DArray* CGLES3Texture2DArrayManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3Texture2DArray(this, name);
		}

		return (CGLES3Texture2DArray *)m_pTextures[name];
	}
}

CGLES3Texture2DArray* CGLES3Texture2DArrayManager::Create(const char *szFileName)
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

void CGLES3Texture2DArrayManager::Destroy(CGfxTexture2DArray *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}


CGLES3TextureCubeMapManager::CGLES3TextureCubeMapManager(void)
{

}

CGLES3TextureCubeMapManager::~CGLES3TextureCubeMapManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

bool CGLES3TextureCubeMapManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CGLES3TextureCubeMap* CGLES3TextureCubeMapManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3TextureCubeMap(this, name);
		}

		return (CGLES3TextureCubeMap *)m_pTextures[name];
	}
}

CGLES3TextureCubeMap* CGLES3TextureCubeMapManager::Create(const char *szFileName)
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

void CGLES3TextureCubeMapManager::Destroy(CGfxTextureCubeMap *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
