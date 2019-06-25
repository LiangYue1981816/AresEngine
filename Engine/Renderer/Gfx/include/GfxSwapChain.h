#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxSwapChain
{
public:
	CGfxSwapChain(int width, int height, GfxPixelFormat format)
	{

	}
	virtual ~CGfxSwapChain(void)
	{

	}


public:
	virtual GfxPixelFormat GetFormat(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;

public:
	virtual int GetFrameIndex(void) const = 0;
	virtual const CGfxRenderTexturePtr GetFrameTexture(int index) const = 0;

public:
	virtual void Present(const CGfxSemaphore* pWaitSemaphore) = 0;
	virtual void AcquireNextFrame(void) = 0;


public:
	static const uint32_t SWAPCHAIN_FRAME_COUNT = 3;
};
