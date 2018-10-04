#pragma once
#include "GfxRenderer.h"


class CGfxTextureManager
{
	friend class CGfxRenderer;


private:
	CGfxTextureManager(void);
	virtual ~CGfxTextureManager(void);


private:
	CGfxTexture2D* LoadTexture2D(const char *szFileName);
	CGfxTexture2DArray* LoadTexture2DArray(const char *szFileName);
	CGfxTextureCubeMap* LoadTextureCubeMap(const char *szFileName);
	CGfxTexture2D* CreateTexture2D(uint32_t name);
	CGfxTexture2DArray* CreateTexture2DArray(uint32_t name);
	CGfxTextureCubeMap* CreateTextureCubeMap(uint32_t name);
	void DestroyTexture(CGfxTextureBase *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTextureBase*> m_pTextures;
};
