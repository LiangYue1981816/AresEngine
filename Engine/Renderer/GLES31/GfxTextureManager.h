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
	void DestroyTexture(CGfxTextureBase *pTexture);


private:
	eastl::unordered_map<GLuint, CGfxTextureBase*> m_pTextures;
};
