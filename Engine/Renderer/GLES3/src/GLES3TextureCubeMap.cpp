#include "GfxHeader.h"


CGLES3TextureCubeMap::CGLES3TextureCubeMap(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3TextureBase(pManager)
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

bool CGLES3TextureCubeMap::Create(uint32_t texture)
{
	return CGLES3TextureBase::Create(GL_TEXTURE_CUBE_MAP, texture);
}

bool CGLES3TextureCubeMap::Create(uint32_t format, int width, int height, int levels)
{
	Destroy();

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format gliFormat = GL.translate((gli::format)format);

	m_format = gliFormat.External;
	m_internalFormat = gliFormat.Internal;

	m_width = width;
	m_height = height;
	m_levels = levels;

	m_target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_levels, m_internalFormat, m_width, m_height);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

void CGLES3TextureCubeMap::Destroy(void)
{
	CGLES3TextureBase::Destroy();

	for (const auto &itFaceSize : m_size) {
		for (const auto &itLevelSize : itFaceSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();
}

bool CGLES3TextureCubeMap::TransferTexture2D(uint32_t format, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data)
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

	if (m_format != gliFormat.External) {
		return false;
	}

	if (m_internalFormat != gliFormat.Internal) {
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = size;
			glTexSubImage2D(face, level, xoffset, yoffset, width, height, m_format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGLES3TextureCubeMap::TransferTexture2DCompressed(uint32_t format, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
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

	if (m_format != gliFormat.External) {
		return false;
	}

	if (m_internalFormat != gliFormat.Internal) {
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = size;
			glCompressedTexSubImage2D(face, level, xoffset, yoffset, width, height, m_format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGLES3TextureCubeMap::IsExtern(void) const
{
	return CGLES3TextureBase::IsExtern();
}

uint32_t CGLES3TextureCubeMap::GetTarget(void) const
{
	return CGLES3TextureBase::GetTarget();
}

uint32_t CGLES3TextureCubeMap::GetTexture(void) const
{
	return CGLES3TextureBase::GetTexture();
}

uint32_t CGLES3TextureCubeMap::GetFormat(void) const
{
	return CGLES3TextureBase::GetFormat();
}
