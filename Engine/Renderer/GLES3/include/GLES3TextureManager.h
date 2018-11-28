#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Texture2D;
	friend class CGLES3Texture2DArray;
	friend class CGLES3TextureCubeMap;


private:
	CGLES3TextureManager(void);
	virtual ~CGLES3TextureManager(void);


private:
	CGLES3Texture2D* CreateTexture2D(const char *szFileName);
	CGLES3Texture2DArray* CreateTexture2DArray(const char *szFileName);
	CGLES3TextureCubeMap* CreateTextureCubeMap(const char *szFileName);
	CGLES3Texture2D* CreateTexture2D(uint32_t name);
	CGLES3Texture2DArray* CreateTexture2DArray(uint32_t name);
	CGLES3TextureCubeMap* CreateTextureCubeMap(uint32_t name);
	void Destroy(CGfxTextureBase *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTextureBase*> m_pTextures;
};
