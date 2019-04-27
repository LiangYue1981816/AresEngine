#include "GLES3Renderer.h"


CGLES3TextureCubemap::CGLES3TextureCubemap(CGLES3TextureCubemapManager* pManager, uint32_t name)
	: CGfxTextureCubemap(name)
	, m_pManager(pManager)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3TextureCubemap::~CGLES3TextureCubemap(void)
{
	Destroy();
}

void CGLES3TextureCubemap::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3TextureCubemap::GetTexture(void) const
{
	return m_ptrTexture->GetTexture();
}

GfxTextureType CGLES3TextureCubemap::GetType(void) const
{
	return m_ptrTexture->GetType();
}

GfxPixelFormat CGLES3TextureCubemap::GetFormat(void) const
{
	return m_ptrTexture->GetFormat();
}

int CGLES3TextureCubemap::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGLES3TextureCubemap::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3TextureCubemap::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

bool CGLES3TextureCubemap::Create(GfxPixelFormat format, int width, int height, int levels)
{
	return m_ptrTexture->Create(GFX_TEXTURE_CUBE_MAP, format, width, height, 1, levels, 1);
}

void CGLES3TextureCubemap::Destroy(void)
{
	m_ptrTexture->Destroy();
}

bool CGLES3TextureCubemap::TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->TextureCubemapData(format, face, level, xoffset, yoffset, width, height, size, data);
}

bool CGLES3TextureCubemap::TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->TextureCubemapDataCompressed(format, face, level, xoffset, yoffset, width, height, size, data);
}

void CGLES3TextureCubemap::Bind(uint32_t unit) const
{
	m_ptrTexture->Bind(unit);
}
