#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureCubeMap : public CGfxTextureCubeMap
{
	friend class CGLES3TextureCubeMapManager;


private:
	CGLES3TextureCubeMap(CGLES3TextureCubeMapManager* pManager, uint32_t name);
	virtual ~CGLES3TextureCubeMap(void);
	virtual void Release(void);


public:
	uint32_t GetTexture(void) const;

public:
	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLevels(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int levels);
	void Destroy(void);

public:
	bool TextureData2D(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TextureData2DCompressed(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

public:
	void Bind(uint32_t unit) const;


private:
	CGLES3TexturePtr m_ptrTexture;

private:
	CGLES3TextureCubeMapManager* m_pManager;
};
