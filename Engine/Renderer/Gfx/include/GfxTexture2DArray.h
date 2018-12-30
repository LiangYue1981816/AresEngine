#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2DArray : public CGfxTexture
{
public:
	CGfxTexture2DArray(uint32_t name)
		: CGfxTexture(name)
		, m_layers(0)
	{

	}
	virtual ~CGfxTexture2DArray(void)
	{

	}


public:
	virtual bool Create(uint32_t texture) = 0;
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int layers) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;

public:
	int GetLayers(void) const
	{
		return m_layers;
	}


protected:
	int m_layers;
};
