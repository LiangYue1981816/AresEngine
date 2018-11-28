#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2D : public CGfxTextureBase
{
public:
	CGfxTexture2D(uint32_t name)
		: CGfxTextureBase(name)
		, m_samples(0)
	{

	}
	virtual ~CGfxTexture2D(void)
	{

	}


public:
	virtual bool Create(uint32_t texture) = 0;
	virtual bool Create(uint32_t format, int width, int height, int levels, int samples = 0) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(uint32_t format, int level, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(uint32_t format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;

public:
	int GetSamples(void) const
	{
		return m_samples;
	}


protected:
	int m_samples;
};
