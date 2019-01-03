#pragma once
#include "GLES3Renderer.h"


class CGLES3RenderTexture : public CGfxRenderTexture
{
	friend class CGLES3RenderTextureManager;


private:
	CGLES3RenderTexture(CGLES3RenderTextureManager *pManager, uint32_t name);
	virtual ~CGLES3RenderTexture(void);
	virtual void Release(void);


public:
	bool Create(GfxPixelFormat pixelFormat, int width, int height, int samples = 1, bool bTransient = false);
	void Destroy(void);

public:
	uint32_t GetTexture(void) const;


private:
	uint32_t m_texture;

private:
	CGLES3RenderTextureManager *m_pManager;
};
