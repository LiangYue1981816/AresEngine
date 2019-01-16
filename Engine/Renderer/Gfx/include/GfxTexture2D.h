#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2D : public CGfxResource
{
public:
	CGfxTexture2D(uint32_t name)
	{

	}
	virtual ~CGfxTexture2D(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual HANDLE GetTexture(void) const = 0;

	virtual GfxPixelFormat GetFormat(void) const = 0;
	virtual GfxTextureType GetType(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetLevels(void) const = 0;
	virtual int GetSamples(void) const = 0;

public:
	virtual bool Create(HANDLE hTexture) = 0;
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples = 1) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;
};
