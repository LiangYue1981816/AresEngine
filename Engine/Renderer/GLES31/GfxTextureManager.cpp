#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTextureManager.h"


CGfxTextureManager::CGfxTextureManager(void)
{

}

CGfxTextureManager::~CGfxTextureManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CGfxTexture2D* CGfxTextureManager::CreateTexture2D(GLuint name)
{
	if (m_pTextures[name] == NULL) {
		m_pTextures[name] = new CGfxTexture2D(name);
	}

	return (CGfxTexture2D *)m_pTextures[name];
}

CGfxTexture2DArray* CGfxTextureManager::CreateTexture2DArray(GLuint name)
{
	if (m_pTextures[name] == NULL) {
		m_pTextures[name] = new CGfxTexture2DArray(name);
	}

	return (CGfxTexture2DArray *)m_pTextures[name];
}

CGfxTextureCubeMap* CGfxTextureManager::CreateTextureCubeMap(GLuint name)
{
	if (m_pTextures[name] == NULL) {
		m_pTextures[name] = new CGfxTextureCubeMap(name);
	}

	return (CGfxTextureCubeMap *)m_pTextures[name];
}

CGfxTexture2D* CGfxTextureManager::LoadTexture2D(const char *szFileName)
{
	GLuint name = HashValue(szFileName);

	if (m_pTextures[name] == NULL) {
		m_pTextures[name] = new CGfxTexture2D(name);
		m_pTextures[name]->Load(szFileName);
	}

	return (CGfxTexture2D *)m_pTextures[name];
}

CGfxTexture2DArray* CGfxTextureManager::LoadTexture2DArray(const char *szFileName)
{
	GLuint name = HashValue(szFileName);

	if (m_pTextures[name] == NULL) {
		m_pTextures[name] = new CGfxTexture2DArray(name);
		m_pTextures[name]->Load(szFileName);
	}

	return (CGfxTexture2DArray *)m_pTextures[name];
}

CGfxTextureCubeMap* CGfxTextureManager::LoadTextureCubeMap(const char *szFileName)
{
	GLuint name = HashValue(szFileName);

	if (m_pTextures[name] == NULL) {
		m_pTextures[name] = new CGfxTextureCubeMap(name);
		m_pTextures[name]->Load(szFileName);
	}

	return (CGfxTextureCubeMap *)m_pTextures[name];
}

void CGfxTextureManager::FreeTexture(CGfxTextureBase *pTexture)
{
	if (pTexture && pTexture->refCount == 0) {
		m_pTextures.erase(pTexture->GetName());
		delete pTexture;
	}
}
