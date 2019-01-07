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
	bool IsHaveTexture2D(uint32_t name);
	bool IsHaveTexture2DArray(uint32_t name);
	bool IsHaveTextureCubeMap(uint32_t name);
	CGLES3Texture2D* CreateTexture2D(uint32_t name);
	CGLES3Texture2D* CreateTexture2D(const char *szFileName);
	CGLES3Texture2DArray* CreateTexture2DArray(uint32_t name);
	CGLES3Texture2DArray* CreateTexture2DArray(const char *szFileName);
	CGLES3TextureCubeMap* CreateTextureCubeMap(uint32_t name);
	CGLES3TextureCubeMap* CreateTextureCubeMap(const char *szFileName);
	void DestroyTexture2D(CGfxTexture2D *pTexture);
	void DestroyTexture2DArray(CGfxTexture2DArray *pTexture);
	void DestroyTextureCubeMap(CGfxTextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2D*> m_pTexture2Ds;
	eastl::unordered_map<uint32_t, CGfxTexture2DArray*> m_pTexture2DArrays;
	eastl::unordered_map<uint32_t, CGfxTextureCubeMap*> m_pTextureCubeMaps;
};
