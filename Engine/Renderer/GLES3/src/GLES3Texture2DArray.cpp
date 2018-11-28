#include "GfxHeader.h"


CGLES3Texture2DArray::CGLES3Texture2DArray(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3TextureBase(pManager)
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

bool CGLES3Texture2DArray::Create(uint32_t texture)
{
	return CGLES3TextureBase::Create(GL_TEXTURE_2D_ARRAY, texture);
}

bool CGLES3Texture2DArray::Create(uint32_t format, int width, int height, int levels, int layers)
{
	Destroy();

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format gliFormat = GL.translate((gli::format)format);

	m_format = gliFormat.External;
	m_internalFormat = gliFormat.Internal;

	m_width = width;
	m_height = height;
	m_levels = levels;
	m_layers = layers;

	m_target = GL_TEXTURE_2D_ARRAY;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_levels, m_internalFormat, m_width, m_height, m_layers);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

void CGLES3Texture2DArray::Destroy(void)
{
	CGLES3TextureBase::Destroy();

	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();
}

bool CGLES3Texture2DArray::TransferTexture2D(uint32_t format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format gliFormat = GL.translate((gli::format)format);

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

	if (m_format != gliFormat.External) {
		return false;
	}

	if (m_internalFormat != gliFormat.Internal) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = size;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, m_format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGLES3Texture2DArray::TransferTexture2DCompressed(uint32_t format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format gliFormat = GL.translate((gli::format)format);

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

	if (m_format != gliFormat.External) {
		return false;
	}

	if (m_internalFormat != gliFormat.Internal) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = size;
			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, m_format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGLES3Texture2DArray::IsExtern(void) const
{
	return CGLES3TextureBase::IsExtern();
}

uint32_t CGLES3Texture2DArray::GetTarget(void) const
{
	return CGLES3TextureBase::GetTarget();
}

uint32_t CGLES3Texture2DArray::GetTexture(void) const
{
	return CGLES3TextureBase::GetTexture();
}

uint32_t CGLES3Texture2DArray::GetFormat(void) const
{
	return CGLES3TextureBase::GetFormat();
}
