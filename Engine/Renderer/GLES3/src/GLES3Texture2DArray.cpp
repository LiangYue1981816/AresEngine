#include "GLES3Renderer.h"


CGLES3Texture2DArray::CGLES3Texture2DArray(CGLES3Texture2DArrayManager* pManager, uint32_t name)
	: CGfxTexture2DArray(name)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	m_pTexture = new CGLES3Texture;
}

CGLES3Texture2DArray::~CGLES3Texture2DArray(void)
{
	delete m_pTexture;
}

void CGLES3Texture2DArray::Release(void)
{
	m_pManager->Destroy(this);
}

GfxTextureType CGLES3Texture2DArray::GetType(void) const
{
	return m_pTexture->GetType();
}

GfxPixelFormat CGLES3Texture2DArray::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CGLES3Texture2DArray::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CGLES3Texture2DArray::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CGLES3Texture2DArray::GetLayers(void) const
{
	return m_pTexture->GetLayers();
}

int CGLES3Texture2DArray::GetLevels(void) const
{
	return m_pTexture->GetLevels();
}

bool CGLES3Texture2DArray::Create(GfxPixelFormat format, int width, int height, int layers, int levels)
{
	return m_pTexture->Create(GFX_TEXTURE_2D_ARRAY, format, width, height, 1, levels, 1);
}

void CGLES3Texture2DArray::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CGLES3Texture2DArray::Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
}

bool CGLES3Texture2DArray::Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DArrayDataCompressed(format, layer, level, xoffset, yoffset, width, height, size, data);
}

void CGLES3Texture2DArray::Bind(uint32_t unit) const
{
	m_pTexture->Bind(unit);
}

void CGLES3Texture2DArray::BindImageTexture(uint32_t unit, int level, int layer) const
{
	m_pTexture->BindImageTexture(unit, level, layer);
}
