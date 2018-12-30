#include "GLES3Renderer.h"


CGLES3TextureCubeMap::CGLES3TextureCubeMap(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3Texture(pManager)
	, CGfxTextureCubeMap(name)
{

}

CGLES3TextureCubeMap::~CGLES3TextureCubeMap(void)
{
	Destroy();
}

void CGLES3TextureCubeMap::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3TextureCubeMap::GetTarget(void) const
{
	return CGLES3Texture::GetTarget();
}

bool CGLES3TextureCubeMap::Create(uint32_t texture)
{
	return CGLES3Texture::Create(GL_TEXTURE_CUBE_MAP, texture);
}

bool CGLES3TextureCubeMap::Create(GfxPixelFormat pixelFormat, int width, int height, int levels)
{
	Destroy();

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_levels = levels;

	m_target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_levels, glFormat.Internal, m_width, m_height);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

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

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_format = GFX_PIXELFORMAT_UNDEFINED;
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
			glTexSubImage2D(GLTextureCubeMapFace(face), level, xoffset, yoffset, width, height, glFormat.External, GLDataType(type), data);
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
			glCompressedTexSubImage2D(GLTextureCubeMapFace(face), level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}
