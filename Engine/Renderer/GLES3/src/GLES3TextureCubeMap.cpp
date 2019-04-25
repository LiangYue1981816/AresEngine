#include "GLES3Renderer.h"


CGLES3TextureCubeMap::CGLES3TextureCubeMap(CGLES3TextureCubeMapManager* pManager, uint32_t name)
	: CGfxTextureCubeMap(name)
	, m_pManager(pManager)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3TextureCubeMap::~CGLES3TextureCubeMap(void)
{
	Destroy();
}

void CGLES3TextureCubeMap::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3TextureCubeMap::GetTexture(void) const
{
	return m_ptrTexture->GetTexture();
}

GfxTextureType CGLES3TextureCubeMap::GetType(void) const
{
	return m_ptrTexture->GetType();
}

GfxPixelFormat CGLES3TextureCubeMap::GetFormat(void) const
{
	return m_ptrTexture->GetFormat();
}

int CGLES3TextureCubeMap::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGLES3TextureCubeMap::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3TextureCubeMap::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

bool CGLES3TextureCubeMap::Create(GfxPixelFormat format, int width, int height, int levels)
{
	return m_ptrTexture->Create(GFX_TEXTURE_CUBE_MAP, format, width, height, 1, levels);
}

void CGLES3TextureCubeMap::Destroy(void)
{
	m_ptrTexture->Destroy();
}

bool CGLES3TextureCubeMap::TextureData2D(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->TextureData2D(format, face, level, xoffset, yoffset, width, height, size, data);
}

bool CGLES3TextureCubeMap::TextureData2DCompressed(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->TextureData2DCompressed(format, face, level, xoffset, yoffset, width, height, size, data);
}

void CGLES3TextureCubeMap::Bind(uint32_t unit) const
{
	m_ptrTexture->Bind(unit);
}
