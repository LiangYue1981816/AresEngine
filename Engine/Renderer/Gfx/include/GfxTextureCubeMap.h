#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTextureCubeMap : public CGfxResource
{
public:
	CGfxTextureCubeMap(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxTextureCubeMap(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual GfxPixelFormat GetFormat(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetLevels(void) const = 0;

public:
	virtual bool Create(GfxPixelFormat format, int width, int height, int levels) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TextureData2D(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;
	virtual bool TextureData2DCompressed(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;


private:
	uint32_t m_name;
};
