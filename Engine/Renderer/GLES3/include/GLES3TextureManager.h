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
	CGLES3Texture2D* Get(uint32_t name);
	CGLES3Texture2D* Create(uint32_t name);
	CGLES3Texture2D* Create(const char *szFileName);
	void Destroy(CGLES3Texture2D *pTexture);


private:
	eastl::unordered_map<uint32_t, CGLES3Texture2D*> m_pTextures;
};


class CGLES3Texture2DArrayManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Texture2DArray;


private:
	CGLES3Texture2DArrayManager(void);
	virtual ~CGLES3Texture2DArrayManager(void);


private:
	CGLES3Texture2DArray* Get(uint32_t name);
	CGLES3Texture2DArray* Create(uint32_t name);
	CGLES3Texture2DArray* Create(const char *szFileName);
	void Destroy(CGLES3Texture2DArray *pTexture);


private:
	eastl::unordered_map<uint32_t, CGLES3Texture2DArray*> m_pTextures;
};


class CGLES3TextureCubeMapManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3TextureCubeMap;


private:
	CGLES3TextureCubeMapManager(void);
	virtual ~CGLES3TextureCubeMapManager(void);


private:
	CGLES3TextureCubeMap* Get(uint32_t name);
	CGLES3TextureCubeMap* Create(uint32_t name);
	CGLES3TextureCubeMap* Create(const char *szFileName);
	void Destroy(CGLES3TextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGLES3TextureCubeMap*> m_pTextures;
};
