#pragma once
#include "GfxRenderer.h"


class CGfxTextureManager
{
	friend class CGfxRenderer;


private:
	CGfxTextureManager(void);
	virtual ~CGfxTextureManager(void);


private:
	CGfxTexture2D* CreateTexture2D(GLuint name);
	CGfxTexture2DArray* CreateTexture2DArray(GLuint name);
	CGfxTextureCubeMap* CreateTextureCubeMap(GLuint name);

	CGfxTexture2D* LoadTexture2D(const char *szFileName);
	CGfxTexture2DArray* LoadTexture2DArray(const char *szFileName);
	CGfxTextureCubeMap* LoadTextureCubeMap(const char *szFileName);

	void FreeTexture(CGfxTextureBase *pTexture);


private:
	eastl::unordered_map<GLuint, CGfxTextureBase*> m_pTextures;
};
