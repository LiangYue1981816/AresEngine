#include "GLES3Renderer.h"


CGLES3Texture2DArray::CGLES3Texture2DArray(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3Texture(pManager)
	, CGfxTexture2DArray(name)
	, m_name(name)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_levels(0)
	, m_layers(0)
{

}

CGLES3Texture2DArray::~CGLES3Texture2DArray(void)
{
	Destroy();
}

void CGLES3Texture2DArray::Release(void)
{
	m_pManager->DestroyTexture2DArray(this);
}

uint32_t CGLES3Texture2DArray::GetName(void) const
{
	return m_name;
}

HANDLE CGLES3Texture2DArray::GetTexture(void) const
{
	return (HANDLE)m_texture;
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
	return m_width;
}

int CGLES3Texture2DArray::GetHeight(void) const
{
	return m_height;
}

int CGLES3Texture2DArray::GetLevels(void) const
{
	return m_levels;
}

int CGLES3Texture2DArray::GetLayers(void) const
{
	return m_layers;
}

bool CGLES3Texture2DArray::Create(HANDLE texture)
{
	Destroy();

	if (CGLES3Texture::Create(GL_TEXTURE_2D_ARRAY, (uint32_t)texture) == false) {
		return false;
	}

	m_type = GFX_TEXTURE_2D_ARRAY;

	return true;
}

bool CGLES3Texture2DArray::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int layers)
{
	Destroy();
	
	if (CGLES3Texture::Create(GL_TEXTURE_2D_ARRAY, pixelFormat, width, height, levels, layers) == false) {
		return false;
	}

	m_format = pixelFormat;
	m_type = GFX_TEXTURE_2D_ARRAY;

	m_width = width;
	m_height = height;
	m_levels = levels;
	m_layers = layers;

	return true;
}

void CGLES3Texture2DArray::Destroy(void)
{
	CGLES3Texture::Destroy();

	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_texture = 0;

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_layers = 0;

	m_size.clear();
}

bool CGLES3Texture2DArray::TransferTexture2D(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	if (m_format != pixelFormat) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == true) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = size;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, glFormat.External, CGLES3Helper::TranslateDataType(type), data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGLES3Texture2DArray::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	if (m_format != pixelFormat) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == true) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = size;
			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, glFormat.Internal, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}
