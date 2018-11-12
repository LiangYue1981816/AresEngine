#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2D : public CGfxResource
{
public:
	CGfxTexture2D(void)
		: m_width(0)
		, m_height(0)
		, m_samples(0)
	{

	}
	virtual ~CGfxTexture2D(void)
	{

	}


public:
	virtual bool Load(const char *szFileName) = 0;
	virtual bool Create(uint32_t format, uint32_t internalFormat, int width, int height, int levels, int samples = 0) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(const gli::texture2d *texture) = 0;
	virtual bool TransferTexture2D(int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;

public:
	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}

	int GetSamples(void) const
	{
		return m_samples;
	}


protected:
	int m_width;
	int m_height;
	int m_samples;
};
