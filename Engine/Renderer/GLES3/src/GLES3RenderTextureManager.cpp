#include "GLES3Renderer.h"


CGLES3RenderTextureManager::CGLES3RenderTextureManager(void)
{

}

CGLES3RenderTextureManager::~CGLES3RenderTextureManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

bool CGLES3RenderTextureManager::IsHaveRenderTexture(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pTextures.find(name) != m_pTextures.end();
	}
}

CGLES3RenderTexture* CGLES3RenderTextureManager::CreateRenderTexture(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pTextures[name] == nullptr) {
			m_pTextures[name] = new CGLES3RenderTexture(this, name);
		}

		return (CGLES3RenderTexture *)m_pTextures[name];
	}
}

void CGLES3RenderTextureManager::Destroy(CGfxRenderTexture *pTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pTexture) {
			m_pTextures.erase(pTexture->GetName());
			delete pTexture;
		}
	}
}
