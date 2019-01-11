#include "GLES3Renderer.h"


CGLES3TextureCubeMap::CGLES3TextureCubeMap(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3Texture(pManager)
	, CGfxTextureCubeMap(name)
	, m_name(name)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_levels(0)
{

}

CGLES3TextureCubeMap::~CGLES3TextureCubeMap(void)
{
	Destroy();
}

void CGLES3TextureCubeMap::Release(void)
{
	m_pManager->DestroyTextureCubeMap(this);
}

uint32_t CGLES3TextureCubeMap::GetName(void) const
{
	return m_name;
}

GfxPixelFormat CGLES3TextureCubeMap::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CGLES3TextureCubeMap::GetType(void) const
{
	return m_type;
}

int CGLES3TextureCubeMap::GetWidth(void) const
{
	return m_width;
}

int CGLES3TextureCubeMap::GetHeight(void) const
{
	return m_height;
}

int CGLES3TextureCubeMap::GetLevels(void) const
{
	return m_levels;
}

bool CGLES3TextureCubeMap::Create(HANDLE texture)
{
	Destroy();

	if (CGLES3Texture::Create(GL_TEXTURE_CUBE_MAP, (uint32_t)texture) == false) {
		return false;
	}

	m_type = GFX_TEXTURE_CUBE_MAP;

	return true;
}

bool CGLES3TextureCubeMap::Create(GfxPixelFormat pixelFormat, int width, int height, int levels)
{
	Destroy();

	if (CGLES3Texture::Create(GL_TEXTURE_CUBE_MAP, pixelFormat, width, height, levels, 0) == false) {
		return false;
	}

	m_format = pixelFormat;
	m_type = GFX_TEXTURE_CUBE_MAP;

	m_width = width;
	m_height = height;
	m_levels = levels;

	return true;
}

void CGLES3TextureCubeMap::Destroy(void)
{
	CGLES3Texture::Destroy();

	for (const auto &itFaceSize : m_size) {
		for (const auto &itLevelSize : itFaceSize.second) {
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
}

bool CGLES3TextureCubeMap::TransferTexture2D(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = size;
			glTexSubImage2D(CGLES3Helper::TranslateTextureCubeMapFace(face), level, xoffset, yoffset, width, height, glFormat.External, CGLES3Helper::TranslateDataType(type), data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGLES3TextureCubeMap::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = size;
			glCompressedTexSubImage2D(CGLES3Helper::TranslateTextureCubeMapFace(face), level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}
