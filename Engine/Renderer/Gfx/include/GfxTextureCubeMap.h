#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTextureCubeMap : public CGfxResource
{
public:
	CGfxTextureCubeMap(void)
		: m_width(0)
		, m_height(0)
	{

	}
	virtual ~CGfxTextureCubeMap(void)
	{

	}


public:
	virtual bool Load(const char *szFileName) = 0;
	virtual bool LoadFace(const char *szFileName, int face) = 0;
	virtual bool Create(uint32_t format, uint32_t internalFormat, int width, int height, int levels) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTextureCubeMap(const gli::texture_cube *texture) = 0;
	virtual bool TransferTexture2D(int face, const gli::texture2d *texture) = 0;
	virtual bool TransferTexture2D(int face, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(int face, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;

public:
	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}


protected:
	int m_width;
	int m_height;
};
