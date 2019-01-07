#pragma once
#include "GLES3Renderer.h"


class CGLES3SwapChain : public CGfxSwapChain
{
	friend class CGLES3Renderer;


private:
	CGLES3SwapChain(void *hDC, int width, int height, GfxPixelFormat pixelFormat);
	virtual ~CGLES3SwapChain(void);


public:
	GfxPixelFormat GetPixelFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	int GetFrameIndex(void) const;
	const CGfxRenderTexturePtr& GetFrameTexture(int index) const;

public:
	void Present(void);
	void AcquireNextFrame(void);


private:
	GfxPixelFormat m_pixelFormat;

	int m_width;
	int m_height;

private:
	void *m_hDC;
	uint32_t m_fbo;

private:
	int m_indexFrame;
	CGfxRenderTexturePtr m_ptrFrameTexture;
};
