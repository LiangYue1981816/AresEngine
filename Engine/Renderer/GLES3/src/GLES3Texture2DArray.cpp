#include "GLES3Renderer.h"


CGLES3Texture2DArray::CGLES3Texture2DArray(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3Texture(pManager)
	, CGfxTexture2DArray(name)
{

}

CGLES3Texture2DArray::~CGLES3Texture2DArray(void)
{
	Destroy();
}

void CGLES3Texture2DArray::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3Texture2DArray::Create(uint64_t texture)
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

	m_size.clear();

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;
	m_texture = 0;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_layers = 0;
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
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, glFormat.External, GLDataType(type), data);
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
