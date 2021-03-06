#pragma once
#include "PreHeader.h"


class CALL_API CGfxTextureCubemap : public CGfxResource
{
public:
	CGfxTextureCubemap(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxTextureCubemap(void)
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
	virtual bool TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;
	virtual bool TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;


private:
	uint32_t m_name;
};
