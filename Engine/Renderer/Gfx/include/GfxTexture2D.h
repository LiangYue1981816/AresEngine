#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2D : public CGfxTexture
{
public:
	CGfxTexture2D(uint32_t name)
		: CGfxTexture(name)
		, m_samples(0)
	{

	}
	virtual ~CGfxTexture2D(void)
	{

	}


public:
	virtual bool Create(uint64_t texture) = 0;
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples = 0) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;

public:
	int GetSamples(void) const
	{
		return m_samples;
	}


protected:
	int m_samples;
};
