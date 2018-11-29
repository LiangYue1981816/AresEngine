#include "GfxHeader.h"


CGfxResourceLoader* CGfxResourceLoader::pInstance = nullptr;
CGfxResourceLoader* CGfxResourceLoader::GetInstance(void)
{
	return pInstance;
}

CGfxResourceLoader::CGfxResourceLoader(void)
{
	pInstance = this;
}

CGfxResourceLoader::~CGfxResourceLoader(void)
{
	pInstance = nullptr;
}

bool CGfxResourceLoader::LoadShader(const char *szFileName, CGfxShader *pShader)
{
	return true;
}

bool CGfxResourceLoader::LoadMesh(const char *szFileName, CGfxMesh *pMesh)
{
	return true;
}

bool CGfxResourceLoader::LoadMaterial(const char *szFileName, CGfxMaterial *pMaterial)
{
	return true;
}

bool CGfxResourceLoader::LoadTexture2D(const char *szFileName, CGfxTexture2D *pTexture2D)
{
	return true;
}

bool CGfxResourceLoader::LoadTexture2DArray(const char *szFileName, CGfxTexture2DArray *pTexture2DArray)
{
	return true;
}

bool CGfxResourceLoader::LoadTexture2DArrayLayer(const char *szFileName, int layer, CGfxTexture2DArray *pTexture2DArray)
{
	return true;
}

bool CGfxResourceLoader::LoadTextureCubeMap(const char *szFileName, CGfxTextureCubeMap *pTextureCubeMap)
{
	return true;
}

bool CGfxResourceLoader::LoadTextureCubeMapFace(const char *szFileName, int face, CGfxTextureCubeMap *pTextureCubeMap)
{
	return true;
}
