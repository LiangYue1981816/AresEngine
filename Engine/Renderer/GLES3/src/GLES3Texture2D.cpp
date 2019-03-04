#include "GLES3Renderer.h"


CGLES3Texture2D::CGLES3Texture2D(CGLES3Texture2DManager *pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_name(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_levels(0)
	, m_samples(0)
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

uint32_t CGLES3Texture2D::GetName(void) const
{
	return m_name;
}

HANDLE CGLES3Texture2D::GetTexture(void) const
{
	return (HANDLE)m_texture;
}

GfxPixelFormat CGLES3Texture2D::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CGLES3Texture2D::GetType(void) const
{
	return m_type;
}

int CGLES3Texture2D::GetWidth(void) const
{
	return m_width;
}

int CGLES3Texture2D::GetHeight(void) const
{
	return m_height;
}

int CGLES3Texture2D::GetLevels(void) const
{
	return m_levels;
}

int CGLES3Texture2D::GetSamples(void) const
{
	return m_samples;
}

bool CGLES3Texture2D::Create(HANDLE hExternTexture)
{
	Destroy();

	if (CGLES3Texture::Create(GL_TEXTURE_2D, (uint32_t)hExternTexture) == false) {
		return false;
	}

	m_type = GFX_TEXTURE_2D;

	return true;
}

bool CGLES3Texture2D::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples)
{
	Destroy();

	samples = std::max(samples, 1);

	if (samples == 1) {
		if (CGLES3Texture::Create(GL_TEXTURE_2D, pixelFormat, width, height, levels, 0, samples) == false) {
			return false;
		}
	}
	else {
		if (CGLES3Texture::Create(GL_TEXTURE_2D_MULTISAMPLE, pixelFormat, width, height, levels, 0, samples) == false) {
			return false;
		}
	}

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_levels = levels;
	m_samples = samples;
	m_type = m_samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

	return true;
}

void CGLES3Texture2D::Destroy(void)
{
	CGLES3Texture::Destroy();

	for (const auto &itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}
	
	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_samples = 0;

	m_size.clear();
}

bool CGLES3Texture2D::TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	if (m_type != GFX_TEXTURE_2D) {
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

	if (m_samples != 1) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = size;
			glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, glFormat.External, CGLES3Helper::TranslateDataType(type), data);
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

	if (m_type != GFX_TEXTURE_2D) {
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

	if (m_samples != 1) {
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
