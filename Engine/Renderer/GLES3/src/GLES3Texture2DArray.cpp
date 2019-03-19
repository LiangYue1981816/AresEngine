#include "GLES3Renderer.h"


CGLES3Texture2DArray::CGLES3Texture2DArray(CGLES3Texture2DArrayManager *pManager, uint32_t name)
	: CGfxTexture2DArray(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3Texture2DArray::~CGLES3Texture2DArray(void)
{
	Destroy();
}

void CGLES3Texture2DArray::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3Texture2DArray::GetTexture(void) const
{
	return (HANDLE)m_ptrTexture->GetTexture();
}

GfxPixelFormat CGLES3Texture2DArray::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CGLES3Texture2DArray::GetType(void) const
{
	return m_type;
}

int CGLES3Texture2DArray::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGLES3Texture2DArray::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3Texture2DArray::GetLayers(void) const
{
	return m_ptrTexture->GetLayers();
}

int CGLES3Texture2DArray::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

bool CGLES3Texture2DArray::Create(GfxPixelFormat format, int width, int height, int levels, int layers)
{
	Destroy();
	{
		do {
			m_format = format;
			m_type = GFX_TEXTURE_2D_ARRAY;

			if (m_ptrTexture->Create(GL_TEXTURE_2D_ARRAY, format, width, height, 1, levels) == false) {
				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CGLES3Texture2DArray::Destroy(void)
{
	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_ptrTexture->Destroy();
}

bool CGLES3Texture2DArray::TransferTexture2D(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	if (m_ptrTexture->TransferTexture2DArray(format, layer, level, xoffset, yoffset, width, height, type, size, data)) {
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		m_size[layer][level] = size;
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Texture2DArray::TransferTexture2DCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	if (m_ptrTexture->TransferTexture2DArrayCompressed(format, layer, level, xoffset, yoffset, width, height, size, data)) {
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		m_size[layer][level] = size;
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
		return true;
	}
	else {
		return false;
	}
}
