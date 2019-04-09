#include "GLES3Renderer.h"


CGLES3RenderTextureManager::CGLES3RenderTextureManager(void)
{

}

CGLES3RenderTextureManager::~CGLES3RenderTextureManager(void)
{
	for (const auto &itRenderTexture : m_pRenderTextures) {
		delete itRenderTexture.second;
	}
}

CGLES3RenderTexture* CGLES3RenderTextureManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto &itRenderTexture = m_pRenderTextures.find(name);

		if (itRenderTexture != m_pRenderTextures.end()) {
			return itRenderTexture->second;
		}
		else {
			return nullptr;
		}
	}
}

CGLES3RenderTexture* CGLES3RenderTextureManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pRenderTextures[name] == nullptr) {
			m_pRenderTextures[name] = new CGLES3RenderTexture(this, name);
		}

		return m_pRenderTextures[name];
	}
}

void CGLES3RenderTextureManager::Destroy(CGLES3RenderTexture *pRenderTexture)
{
	mutex_autolock autolock(&lock);
	{
		if (pRenderTexture) {
			if (m_pRenderTextures.find(pRenderTexture->GetName()) != m_pRenderTextures.end()) {
				m_pRenderTextures.erase(pRenderTexture->GetName());
				delete pRenderTexture;
			}
		}
	}
}
