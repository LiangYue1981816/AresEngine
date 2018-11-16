#pragma once
#include "GLES3Renderer.h"


class CGLES3SwapChain : public CGfxSwapChain
{
	friend class CGLES3Renderer;


private:
	CGLES3SwapChain(void *hDC, int width, int height, uint32_t format);
	virtual ~CGLES3SwapChain(void);


public:
	void Present(void);
	void AcquireNextTexture(void);

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
};
