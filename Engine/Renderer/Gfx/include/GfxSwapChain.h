#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxSwapChain
{
public:
	CGfxSwapChain(int width, int height, GfxPixelFormat pixelFormat)
	{

	}
	virtual ~CGfxSwapChain(void)
	{

	}


public:
	virtual GfxPixelFormat GetPixelFormat(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;

public:
	virtual int GetFrameIndex(void) const = 0;
	virtual CGfxRenderTexturePtr GetFrameTexture(int index) const = 0;

public:
	virtual void Present(void) = 0;
	virtual void AcquireNextFrame(void) = 0;


public:
	static const uint32_t SWAPCHAIN_FRAME_COUNT = 3;
};
