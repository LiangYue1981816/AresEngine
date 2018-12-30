#pragma once
#include "GLES3Renderer.h"


class CGLES3SwapChain : public CGfxSwapChain
{
	friend class CGLES3Renderer;


private:
	CGLES3SwapChain(void *hDC, int width, int height, GfxPixelFormat pixelFormat);
	virtual ~CGLES3SwapChain(void);


public:
	void Present(void);
	void AcquireNextFrame(void);

public:
	int GetFrameIndex(void) const;
	const CGfxRenderTexturePtr& GetFrameTexture(int index) const;


private:
	void *m_hDC;
	uint32_t m_fbo;

private:
	int m_indexFrame;
	CGfxRenderTexturePtr m_ptrFrameTexture;
};
