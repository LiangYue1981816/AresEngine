#include "GLES3Renderer.h"


CGLES3Texture::CGLES3Texture(void)
	: m_bExtern(false)

	, m_target(0)
	, m_texture(0)

	, m_type(GFX_TEXTURE_INVALID_ENUM)
	, m_format(GFX_PIXELFORMAT_UNDEFINED)

	, m_width(0)
	, m_height(0)
	, m_layers(0)
	, m_levels(0)
	, m_samples(0)
{

}

CGLES3Texture::~CGLES3Texture(void)
{
	Destroy();
}

void CGLES3Texture::Release(void)
{
	delete this;
}

uint32_t CGLES3Texture::GetTarget(void) const
{
	return m_target;
}

uint32_t CGLES3Texture::GetTexture(void) const
{
	return m_texture;
}

GfxTextureType CGLES3Texture::GetType(void) const
{
	return m_type;
}

GfxPixelFormat CGLES3Texture::GetFormat(void) const
{
	return m_format;
}

int CGLES3Texture::GetWidth(void) const
{
	return m_width;
}

int CGLES3Texture::GetHeight(void) const
{
	return m_height;
}

int CGLES3Texture::GetLayers(void) const
{
	return m_layers;
}

int CGLES3Texture::GetLevels(void) const
{
	return m_levels;
}

int CGLES3Texture::GetSamples(void) const
{
	return m_samples;
}

bool CGLES3Texture::Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, uint32_t texture)
{
	ASSERT(texture);

	Destroy();

	m_bExtern = true;
	m_target = CGLES3Helper::TranslateTextureTarget(type);
	m_texture = texture;

	m_type = type;
	m_format = format;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;
	m_samples = samples;

	return true;
}

bool CGLES3Texture::Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples)
{
	Destroy();

	m_bExtern = false;
	m_target = CGLES3Helper::TranslateTextureTarget(type);
	m_texture = 0;

	m_type = type;
	m_format = format;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	switch (m_target) {
	case GL_TEXTURE_2D_MULTISAMPLE:
		m_samples = samples;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, glFormat.Internal, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		return true;

	case GL_TEXTURE_2D:
		m_samples = 1;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, levels, glFormat.Internal, width, height);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;

	case GL_TEXTURE_2D_ARRAY:
		m_samples = 1;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, glFormat.Internal, width, height, layers);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		return true;

	case GL_TEXTURE_CUBE_MAP:
		m_samples = 1;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, glFormat.Internal, width, height);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return true;

	default:
		Destroy();
		return false;
	}
}

void CGLES3Texture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	m_bExtern = false;
	m_target = 0;
	m_texture = 0;

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_width = 0;
	m_height = 0;
	m_layers = 0;
	m_levels = 0;
	m_samples = 0;
}

bool CGLES3Texture::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(size);
	ASSERT(data);
	ASSERT(m_texture);
	ASSERT(m_bExtern == false);
	ASSERT(m_target == GL_TEXTURE_2D);
	ASSERT(m_format == format);
	ASSERT(m_levels > level);
	ASSERT(m_samples == 1);
	ASSERT(xoffset >= 0 && width >= 0 && xoffset + width < m_width);
	ASSERT(yoffset >= 0 && height >= 0 && yoffset + height < m_height);

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glTexSubImage2D(m_target, level, xoffset, yoffset, width, height, glFormat.External, GL_UNSIGNED_BYTE, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(size);
	ASSERT(data);
	ASSERT(m_texture);
	ASSERT(m_bExtern == false);
	ASSERT(m_target == GL_TEXTURE_2D);
	ASSERT(m_format == format);
	ASSERT(m_levels > level);
	ASSERT(m_samples == 1);
	ASSERT(xoffset >= 0 && width >= 0 && xoffset + width < m_width);
	ASSERT(yoffset >= 0 && height >= 0 && yoffset + height < m_height);

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glCompressedTexSubImage2D(m_target, level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(size);
	ASSERT(data);
	ASSERT(m_texture);
	ASSERT(m_bExtern == false);
	ASSERT(m_target == GL_TEXTURE_2D_ARRAY);
	ASSERT(m_format == format);
	ASSERT(m_layers > layer);
	ASSERT(m_levels > level);
	ASSERT(m_samples == 1);
	ASSERT(xoffset >= 0 && width >= 0 && xoffset + width < m_width);
	ASSERT(yoffset >= 0 && height >= 0 && yoffset + height < m_height);

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glTexSubImage3D(m_target, level, xoffset, yoffset, layer, width, height, 1, glFormat.External, GL_UNSIGNED_BYTE, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(size);
	ASSERT(data);
	ASSERT(m_texture);
	ASSERT(m_bExtern == false);
	ASSERT(m_target == GL_TEXTURE_2D_ARRAY);
	ASSERT(m_format == format);
	ASSERT(m_layers > layer);
	ASSERT(m_levels > level);
	ASSERT(m_samples == 1);
	ASSERT(xoffset >= 0 && width >= 0 && xoffset + width < m_width);
	ASSERT(yoffset >= 0 && height >= 0 && yoffset + height < m_height);

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glCompressedTexSubImage3D(m_target, level, xoffset, yoffset, layer, width, height, 1, glFormat.Internal, size, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(size);
	ASSERT(data);
	ASSERT(m_texture);
	ASSERT(m_bExtern == false);
	ASSERT(m_target == GL_TEXTURE_CUBE_MAP);
	ASSERT(m_format == format);
	ASSERT(m_levels > level);
	ASSERT(m_samples == 1);
	ASSERT(xoffset >= 0 && width >= 0 && xoffset + width < m_width);
	ASSERT(yoffset >= 0 && height >= 0 && yoffset + height < m_height);

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glTexSubImage2D(CGLES3Helper::TranslateTextureTarget(face), level, xoffset, yoffset, width, height, glFormat.External, GL_UNSIGNED_BYTE, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(size);
	ASSERT(data);
	ASSERT(m_texture);
	ASSERT(m_bExtern == false);
	ASSERT(m_target == GL_TEXTURE_CUBE_MAP);
	ASSERT(m_format == format);
	ASSERT(m_levels > level);
	ASSERT(m_samples == 1);
	ASSERT(xoffset >= 0 && width >= 0 && xoffset + width < m_width);
	ASSERT(yoffset >= 0 && height >= 0 && yoffset + height < m_height);

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glCompressedTexSubImage2D(CGLES3Helper::TranslateTextureTarget(face), level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
	glBindTexture(m_target, 0);

	return true;
}

void CGLES3Texture::Bind(uint32_t unit) const
{
	ASSERT(m_texture);
	GLBindTexture(unit, m_target, m_texture);
}
