#pragma once
#include "GfxRenderer.h"


class CGfxTextureBase : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxTextureManager;


protected:
	CGfxTextureBase(uint32_t name);
	virtual ~CGfxTextureBase(void);
	virtual void Release(void);

public:
	uint32_t GetName(void) const;


public:
	virtual bool Load(const char *szFileName) = 0;
	virtual bool CreateExtern(uint32_t texture);
	virtual void Destroy(void);

public:
	virtual uint32_t GetTarget(void) const = 0;
	virtual uint32_t GetTexture(void) const;

	virtual uint32_t GetFormat(void) const;
	virtual uint32_t GetInternalFormat(void) const;

	virtual int GetWidth(void) const;
	virtual int GetHeight(void) const;

	virtual int GetLevels(void) const;
	virtual int GetLayers(void) const;


protected:
	uint32_t m_name;

protected:
	uint32_t m_texture;
	uint32_t m_bExtern;

protected:
	uint32_t m_format;
	uint32_t m_internalFormat;

	int m_width;
	int m_height;

	int m_samples;
	int m_mipLevels;
	int m_arrayLayers;
};
