#include "GLES3Renderer.h"


CGLES3TextureCubemap::CGLES3TextureCubemap(CGLES3TextureCubemapManager* pManager, uint32_t name)
	: CGfxTextureCubemap(name)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	ASSERT(m_pManager);

	m_pTexture = new CGLES3Texture;
}

CGLES3TextureCubemap::~CGLES3TextureCubemap(void)
{
	delete m_pTexture;
}

void CGLES3TextureCubemap::Release(void)
{
	m_pManager->Destroy(this);
}

GfxTextureType CGLES3TextureCubemap::GetType(void) const
{
	return m_pTexture->GetType();
}

GfxPixelFormat CGLES3TextureCubemap::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CGLES3TextureCubemap::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CGLES3TextureCubemap::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CGLES3TextureCubemap::GetLevels(void) const
{
	return m_pTexture->GetLevels();
}

bool CGLES3TextureCubemap::Create(GfxPixelFormat format, int width, int height, int levels)
{
	return m_pTexture->Create(GFX_TEXTURE_CUBE_MAP, format, width, height, 1, levels, 1);
}

void CGLES3TextureCubemap::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CGLES3TextureCubemap::TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->TextureCubemapData(format, face, level, xoffset, yoffset, width, height, size, data);
}

bool CGLES3TextureCubemap::TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->TextureCubemapDataCompressed(format, face, level, xoffset, yoffset, width, height, size, data);
}

void CGLES3TextureCubemap::Bind(uint32_t unit) const
{
	m_pTexture->Bind(unit);
}
