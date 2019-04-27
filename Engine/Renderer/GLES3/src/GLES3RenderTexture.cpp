#include "GLES3Renderer.h"


CGLES3RenderTexture::CGLES3RenderTexture(CGLES3RenderTextureManager* pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pManager(pManager)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3RenderTexture::~CGLES3RenderTexture(void)
{
	Destroy();
}

void CGLES3RenderTexture::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3RenderTexture::GetTexture(void) const
{
	return m_ptrTexture->GetTexture();
}

GfxTextureType CGLES3RenderTexture::GetType(void) const
{
	return m_ptrTexture->GetType();
}

GfxPixelFormat CGLES3RenderTexture::GetFormat(void) const
{
	return m_ptrTexture->GetFormat();
}

int CGLES3RenderTexture::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGLES3RenderTexture::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3RenderTexture::GetSamples(void) const
{
	return m_ptrTexture->GetSamples();
}

bool CGLES3RenderTexture::Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples)
{
	samples = std::max(samples, 1);
	return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, (uint32_t)hExternalTexture);
}

bool CGLES3RenderTexture::Create(GfxPixelFormat format, int width, int height, int samples, bool bTransient)
{
	samples = std::max(samples, 1);
	return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples);
}

void CGLES3RenderTexture::Destroy(void)
{
	m_ptrTexture->Destroy();
}

void CGLES3RenderTexture::Bind(uint32_t unit) const
{
	m_ptrTexture->Bind(unit);
}
