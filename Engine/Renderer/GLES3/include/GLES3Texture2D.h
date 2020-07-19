#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2D : public CGfxTexture2D
{
	friend class CGLES3Texture2DManager;


private:
	CGLES3Texture2D(CGLES3Texture2DManager* pManager, uint32_t name);
	virtual ~CGLES3Texture2D(void);
	virtual void Release(void);


public:
	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int levels, int samples = 1);
	void Destroy(void);

public:
	bool Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

public:
	void Bind(uint32_t unit) const;
	void BindImageTexture(uint32_t unit, int level) const;


private:
	CGLES3Texture* m_pTexture;

private:
	CGLES3Texture2DManager* m_pManager;
};
