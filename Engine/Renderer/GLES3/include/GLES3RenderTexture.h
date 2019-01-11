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
	uint32_t GetName(void) const;

public:
	HANDLE GetTexture(void) const;

	GfxPixelFormat GetFormat(void) const;
	GfxTextureType GetType(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetSamples(void) const;

public:
	bool Create(GfxPixelFormat pixelFormat, int width, int height, int samples = 1, bool bTransient = false);
	void Destroy(void);


private:
	uint32_t m_name;

private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_samples;

private:
	uint32_t m_texture;

private:
	CGLES3RenderTextureManager *m_pManager;
};
