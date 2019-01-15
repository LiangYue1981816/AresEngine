#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Texture2D;


private:
	CGLES3Texture2DManager(void);
	virtual ~CGLES3Texture2DManager(void);


private:
	bool IsHave(uint32_t name);
	CGLES3Texture2D* Create(uint32_t name);
	CGLES3Texture2D* Create(const char *szFileName);
	void Destroy(CGfxTexture2D *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2D*> m_pTextures;
};


class CGLES3Texture2DArrayManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Texture2DArray;


private:
	CGLES3Texture2DArrayManager(void);
	virtual ~CGLES3Texture2DArrayManager(void);


private:
	bool IsHave(uint32_t name);
	CGLES3Texture2DArray* Create(uint32_t name);
	CGLES3Texture2DArray* Create(const char *szFileName);
	void Destroy(CGfxTexture2DArray *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2DArray*> m_pTextures;
};


class CGLES3TextureCubeMapManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3TextureCubeMap;


private:
	CGLES3TextureCubeMapManager(void);
	virtual ~CGLES3TextureCubeMapManager(void);


private:
	bool IsHave(uint32_t name);
	CGLES3TextureCubeMap* Create(uint32_t name);
	CGLES3TextureCubeMap* Create(const char *szFileName);
	void Destroy(CGfxTextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTextureCubeMap*> m_pTextures;
};
