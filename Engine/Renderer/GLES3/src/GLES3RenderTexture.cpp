#include "GLES3Renderer.h"


CGLES3RenderTexture::CGLES3RenderTexture(CGLES3RenderTextureManager* pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	m_pTexture = new CGLES3Texture;
}

CGLES3RenderTexture::~CGLES3RenderTexture(void)
{
	delete m_pTexture;
}

void CGLES3RenderTexture::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3RenderTexture::GetTexture(void) const
{
	return m_pTexture->GetTexture();
}

GfxTextureType CGLES3RenderTexture::GetType(void) const
{
	return m_pTexture->GetType();
}

GfxPixelFormat CGLES3RenderTexture::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CGLES3RenderTexture::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CGLES3RenderTexture::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CGLES3RenderTexture::GetSamples(void) const
{
	return m_pTexture->GetSamples();
}

bool CGLES3RenderTexture::Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples)
{
	samples = std::max(samples, 1);
	return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, (uint32_t)hExternalTexture);
}

bool CGLES3RenderTexture::Create(GfxPixelFormat format, int width, int height, int samples, bool bTransient)
{
	samples = std::max(samples, 1);
	return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples);
}

void CGLES3RenderTexture::Destroy(void)
{
	m_pTexture->Destroy();
}

void CGLES3RenderTexture::Bind(uint32_t unit) const
{
	m_pTexture->Bind(unit);
}

void CGLES3RenderTexture::BindImageTexture(uint32_t unit) const
{
	m_pTexture->BindImageTexture(unit, 0, -1);
}
