#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxRenderTexture : public CGfxResource
{
public:
	CGfxRenderTexture(uint32_t name)
	{

	}
	virtual ~CGfxRenderTexture(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual GfxPixelFormat GetFormat(void) const = 0;
	virtual GfxTextureType GetType(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetSamples(void) const = 0;

public:
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int samples = 1, bool bTransient = false) = 0;
	virtual void Destroy(void) = 0;
};
