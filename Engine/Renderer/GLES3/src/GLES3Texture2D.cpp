#include "GLES3Renderer.h"


CGLES3Texture2D::CGLES3Texture2D(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3Texture(pManager)
	, CGfxTexture2D(name)
{

}

CGLES3Texture2D::~CGLES3Texture2D(void)
{
	Destroy();
}

void CGLES3Texture2D::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3Texture2D::GetTarget(void) const
{
	return CGLES3Texture::GetTarget();
}

bool CGLES3Texture2D::Create(uint32_t texture)
{
	return CGLES3Texture::Create(GL_TEXTURE_2D, texture);
}

bool CGLES3Texture2D::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples)
{
	Destroy();

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_levels = levels;
	m_samples = samples;

#if GLES_VER == 310
	if (m_samples == 0)
#endif
	{
		m_target = GL_TEXTURE_2D;

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, m_levels, glFormat.Internal, m_width, m_height);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
#if GLES_VER == 310
	else
	{
		m_target = GL_TEXTURE_2D_MULTISAMPLE;

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, glFormat.Internal, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
#endif

	return true;
}

void CGLES3Texture2D::Destroy(void)
{
	CGLES3Texture::Destroy();

	for (const auto &itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}

	m_size.clear();

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_samples = 0;
	m_format = GFX_PIXELFORMAT_UNDEFINED;
}

bool CGLES3Texture2D::TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	if (m_target != GL_TEXTURE_2D) {
		return false;
	}

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

	if (m_samples != 0) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = size;
			glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, glFormat.External, GLDataType(type), data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[level]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool CGLES3Texture2D::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	if (m_target != GL_TEXTURE_2D) {
		return false;
	}

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

	if (m_samples != 0) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = size;
			glCompressedTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[level]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
