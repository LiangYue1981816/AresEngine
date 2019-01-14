#include "GLES3Renderer.h"
#include "ResourceLoader.h"


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
