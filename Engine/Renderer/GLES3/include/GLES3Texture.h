#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture
{
protected:
	CGLES3Texture(void);
	virtual ~CGLES3Texture(void);


protected:
	bool Create(uint32_t target, uint32_t texture);
	bool Create(uint32_t target, GfxPixelFormat format, int width, int height, int levels, int layers, int samples = 1);
	void Destroy(void);

public:
	void Bind(uint32_t unit) const;


protected:
	bool m_bExtern;
	uint32_t m_target;
	uint32_t m_texture;
};
