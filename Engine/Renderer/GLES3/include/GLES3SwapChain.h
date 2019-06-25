#pragma once
#include "GLES3Renderer.h"


class CGLES3SwapChain : public CGfxSwapChain
{
	friend class CGLES3Renderer;


private:
	CGLES3SwapChain(void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CGLES3SwapChain(void);


private:
	bool Create(int width, int height, GfxPixelFormat format);
	void Destroy(void);

public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	int GetFrameIndex(void) const;
	const CGfxRenderTexturePtr GetFrameTexture(int index) const;

public:
	void Present(CGfxSemaphore* pWaitSemaphore);
	void AcquireNextFrame(void);


#ifdef PLATFORM_WINDOWS
private:
	void* m_hDC;
#endif

private:
	uint32_t m_surface;

private:
	GfxPixelFormat m_format;

	int m_width;
	int m_height;

	int m_indexFrame;

private:
	CGfxRenderTexturePtr m_ptrFrameTexture;
};
