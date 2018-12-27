#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxSwapChain
{
public:
	CGfxSwapChain(int width, int height, GfxPixelFormat pixelFormat)
		: m_width(width)
		, m_height(height)
		, m_pixelFormat(pixelFormat)
	{

	}
	virtual ~CGfxSwapChain(void)
	{

	}


public:
	virtual void Present(void) = 0;
	virtual void AcquireNextTexture(void) = 0;

public:
	virtual int GetTextureIndex(void) const = 0;
	virtual const CGfxTexture2DPtr& GetTexture(int index) const = 0;

public:
	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}

	GfxPixelFormat GetPixelFormat(void) const
	{
		return m_pixelFormat;
	}


protected:
	int m_width;
	int m_height;
	GfxPixelFormat m_pixelFormat;

public:
	static const uint32_t SWAPCHAIN_IMAGE_COUNT = 3;
};
