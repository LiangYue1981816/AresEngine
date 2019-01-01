#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTextureCubeMap : public CGfxTexture
{
public:
	CGfxTextureCubeMap(uint32_t name)
		: CGfxTexture(name)
	{

	}
	virtual ~CGfxTextureCubeMap(void)
	{

	}


public:
	virtual bool Create(uint64_t texture) = 0;
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;
};
