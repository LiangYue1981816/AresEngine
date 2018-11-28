#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTextureCubeMap : public CGfxTextureBase
{
public:
	CGfxTextureCubeMap(uint32_t name)
		: CGfxTextureBase(name)
	{

	}
	virtual ~CGfxTextureCubeMap(void)
	{

	}


public:
	virtual bool Create(uint32_t texture) = 0;
	virtual bool Create(uint32_t format, int width, int height, int levels) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(uint32_t format, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(uint32_t format, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;
};
