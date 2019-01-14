#include "GLES3Renderer.h"
#include "ResourceLoader.h"


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
