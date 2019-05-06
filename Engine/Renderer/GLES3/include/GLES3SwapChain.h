#pragma once
#include "GLES3Renderer.h"


class CGLES3SwapChain : public CGfxSwapChain
{
	friend class CGLES3Renderer;


private:
	CGLES3SwapChain(void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CGLES3SwapChain(void);


public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	int GetFrameIndex(void) const;
	const CGfxRenderTexturePtr GetFrameTexture(int index) const;

public:
	void Present(void);
	void AcquireNextFrame(void);


private:
	void* m_hDC;

private:
	uint32_t m_surface;

private:
	GfxPixelFormat m_format;

	int m_width;
	int m_height;
	int m_indexFrame;

	CGfxRenderTexturePtr m_ptrFrameTexture;
};
