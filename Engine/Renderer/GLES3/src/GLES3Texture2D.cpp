#include "GLES3Renderer.h"


CGLES3Texture2D::CGLES3Texture2D(CGLES3Texture2DManager* pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	m_pTexture = new CGLES3Texture;
}

CGLES3Texture2D::~CGLES3Texture2D(void)
{
	delete m_pTexture;
}

void CGLES3Texture2D::Release(void)
{
	m_pManager->Destroy(this);
}

GfxTextureType CGLES3Texture2D::GetType(void) const
{
	return m_pTexture->GetType();
}

GfxPixelFormat CGLES3Texture2D::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CGLES3Texture2D::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CGLES3Texture2D::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CGLES3Texture2D::GetLevels(void) const
{
	return m_pTexture->GetLevels();
}

int CGLES3Texture2D::GetSamples(void) const
{
	return m_pTexture->GetSamples();
}

bool CGLES3Texture2D::Create(GfxPixelFormat format, int width, int height, int levels, int samples)
{
	samples = std::max(samples, 1);
	return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, levels, samples);
}

void CGLES3Texture2D::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CGLES3Texture2D::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
}

bool CGLES3Texture2D::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DDataCompressed(format, level, xoffset, yoffset, width, height, size, data);
}

void CGLES3Texture2D::Bind(uint32_t unit) const
{
	m_pTexture->Bind(unit);
}
