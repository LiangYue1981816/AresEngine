#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureBase
{
	friend class CGLES3TextureManager;


protected:
	CGLES3TextureBase(CGLES3TextureManager *pManager);
	virtual ~CGLES3TextureBase(void);


public:
	bool Create(uint32_t target, uint32_t texture);
	void Destroy(void);

public:
	void Bind(uint32_t unit);

public:
	bool IsExtern(void) const;

	uint32_t GetTarget(void) const;
	uint32_t GetTexture(void) const;

	GfxPixelFormat GetFormat(void) const;


protected:
	bool m_bExtern;

	uint32_t m_target;
	uint32_t m_texture;

	GfxPixelFormat m_format;

protected:
	CGLES3TextureManager *m_pManager;
};
