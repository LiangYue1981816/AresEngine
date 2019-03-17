#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTextureCubeMap : public CGfxResource
{
public:
	CGfxTextureCubeMap(uint32_t name)
	{

	}
	virtual ~CGfxTextureCubeMap(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual HANDLE GetTexture(void) const = 0;

public:
	virtual GfxPixelFormat GetFormat(void) const = 0;
	virtual GfxTextureType GetType(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetLevels(void) const = 0;

public:
	virtual bool Create(GfxPixelFormat format, int width, int height, int levels) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(GfxPixelFormat format, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(GfxPixelFormat format, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;
};
