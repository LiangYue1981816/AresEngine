#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture
{
	friend class CGLES3TextureManager;


protected:
	CGLES3Texture(CGLES3TextureManager *pManager);
	virtual ~CGLES3Texture(void);


protected:
	bool Create(uint32_t target, uint32_t texture);
	bool Create(uint32_t target, GfxPixelFormat pixelFormat, int width, int height, int levels, int layers, int samples = 1);
	void Destroy(void);

public:
	void Bind(uint32_t unit);

public:
	uint32_t GetTexture(void) const;


protected:
	bool m_bExtern;
	uint32_t m_target;
	uint32_t m_texture;

protected:
	CGLES3TextureManager *m_pManager;
};
