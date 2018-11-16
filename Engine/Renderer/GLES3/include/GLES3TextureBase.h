#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureBase : public CGfxTextureBase
{
	friend class CGLES3TextureManager;


protected:
	CGLES3TextureBase(CGLES3TextureManager *pManager, uint32_t name);
	virtual ~CGLES3TextureBase(void);


public:
	virtual bool Create(uint32_t texture);
	virtual void Destroy(void);

public:
	virtual void Bind(uint32_t unit);

public:
	virtual uint32_t GetTarget(void) const = 0;
	virtual uint32_t GetTexture(void) const;

	virtual uint32_t GetFormat(void) const;
	virtual uint32_t GetInternalFormat(void) const;


protected:
	uint32_t m_texture;
	uint32_t m_bExtern;

	uint32_t m_format;
	uint32_t m_internalFormat;

	int m_levels;

protected:
	CGLES3TextureManager *m_pManager;
};
