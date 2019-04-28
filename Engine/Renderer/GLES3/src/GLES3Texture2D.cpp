#include "GLES3Renderer.h"


CGLES3Texture2D::CGLES3Texture2D(CGLES3Texture2DManager* pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_pManager(pManager)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3Texture2D::~CGLES3Texture2D(void)
{
	Destroy();
}

void CGLES3Texture2D::Release(void)
{
	m_pManager->Destroy(this);
}

GfxTextureType CGLES3Texture2D::GetType(void) const
{
	return m_ptrTexture->GetType();
}

GfxPixelFormat CGLES3Texture2D::GetFormat(void) const
{
	return m_ptrTexture->GetFormat();
}

int CGLES3Texture2D::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGLES3Texture2D::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3Texture2D::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

int CGLES3Texture2D::GetSamples(void) const
{
	return m_ptrTexture->GetSamples();
}

bool CGLES3Texture2D::Create(GfxPixelFormat format, int width, int height, int levels, int samples)
{
	samples = std::max(samples, 1);
	return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, levels, samples);
}

void CGLES3Texture2D::Destroy(void)
{
	m_ptrTexture->Destroy();
}

bool CGLES3Texture2D::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
}

bool CGLES3Texture2D::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->Texture2DDataCompressed(format, level, xoffset, yoffset, width, height, size, data);
}

void CGLES3Texture2D::Bind(uint32_t unit) const
{
	m_ptrTexture->Bind(unit);
}
