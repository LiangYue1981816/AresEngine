#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureCubemap : public CGfxTextureCubemap
{
	friend class CGLES3TextureCubemapManager;


private:
	CGLES3TextureCubemap(CGLES3TextureCubemapManager* pManager, uint32_t name);
	virtual ~CGLES3TextureCubemap(void);
	virtual void Release(void);


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
	bool TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

public:
	void Bind(uint32_t unit) const;
	void BindImageTexture(uint32_t unit, int level, int layer) const;


private:
	CGLES3Texture* m_pTexture;

private:
	CGLES3TextureCubemapManager* m_pManager;
};
