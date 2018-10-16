#pragma once
#include "GfxRenderer.h"


class CGfxSwapChain
{
	friend class CGfxRenderer;


private:
	CGfxSwapChain(void *hDC, int width, int height, uint32_t format);
	virtual ~CGfxSwapChain(void);


private:
	void Present(void);

public:
	int GetTextureIndex(void) const;
	const CGfxTexture2DPtr& GetTexture(int index) const;


private:
	void *m_hDC;

private:
	uint32_t m_fbo;

private:
	int m_indexTexture;
	CGfxTexture2DPtr m_ptrTexture;

public:
	static const uint32_t SWAPCHAIN_IMAGE_COUNT = 3;
};
