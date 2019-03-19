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
	HANDLE GetTexture(void) const;

	GfxPixelFormat GetFormat(void) const;
	GfxTextureType GetType(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetSamples(void) const;

public:
	bool Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples = 1);
	bool Create(GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);
	void Destroy(void);

public:
	void Bind(uint32_t unit) const;


private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	CGLES3TexturePtr m_ptrTexture;

private:
	CGLES3RenderTextureManager *m_pManager;
};
