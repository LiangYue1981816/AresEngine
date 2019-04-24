#include "GLES3Renderer.h"


CGLES3Texture::CGLES3Texture(void)
	: m_bExtern(false)
	, m_target(0)
	, m_texture(0)

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

bool CGLES3Texture::Create(uint32_t target, uint32_t texture, int width, int height, int layers, int levels, int samples)
{
	Destroy();

	m_bExtern = true;
	m_target = target;
	m_texture = texture;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;
	m_samples = samples;

	return true;
}

bool CGLES3Texture::Create(uint32_t target, GfxPixelFormat format, int width, int height, int layers, int levels, int samples)
{
	Destroy();

	m_bExtern = false;
	m_target = target;
	m_texture = 0;

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
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, glFormat.Internal, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		return true;

	case GL_TEXTURE_2D:
		m_samples = 1;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, m_levels, glFormat.Internal, m_width, m_height);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;

	case GL_TEXTURE_2D_ARRAY:
		m_samples = 1;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_levels, glFormat.Internal, m_width, m_height, m_layers);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		return true;

	case GL_TEXTURE_CUBE_MAP:
		m_samples = 1;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_levels, glFormat.Internal, m_width, m_height);
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

	m_width = 0;
	m_height = 0;
	m_layers = 0;
	m_levels = 0;
	m_samples = 0;
}

bool CGLES3Texture::TransferTexture2D(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_target != GL_TEXTURE_2D) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glTexSubImage2D(m_target, level, xoffset, yoffset, width, height, glFormat.External, CGLES3Helper::TranslateDataType(type), data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TransferTexture2DCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_target != GL_TEXTURE_2D) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glCompressedTexSubImage2D(m_target, level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TransferTexture2DArray(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_target != GL_TEXTURE_2D_ARRAY) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glTexSubImage3D(m_target, level, xoffset, yoffset, layer, width, height, 1, glFormat.External, CGLES3Helper::TranslateDataType(type), data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TransferTexture2DArrayCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_target != GL_TEXTURE_2D_ARRAY) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glCompressedTexSubImage3D(m_target, level, xoffset, yoffset, layer, width, height, 1, glFormat.Internal, size, data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TransferTextureCubeMap(GfxPixelFormat format, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_target != GL_TEXTURE_CUBE_MAP) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glTexSubImage2D(CGLES3Helper::TranslateTextureCubeMapFace(face), level, xoffset, yoffset, width, height, glFormat.External, CGLES3Helper::TranslateDataType(type), data);
	glBindTexture(m_target, 0);

	return true;
}

bool CGLES3Texture::TransferTextureCubeMapCompressed(GfxPixelFormat format, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_target != GL_TEXTURE_CUBE_MAP) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)format);

	glBindTexture(m_target, m_texture);
	glCompressedTexSubImage2D(CGLES3Helper::TranslateTextureCubeMapFace(face), level, xoffset, yoffset, width, height, glFormat.Internal, size, data);
	glBindTexture(m_target, 0);

	return true;
}

void CGLES3Texture::Bind(uint32_t unit) const
{
	GLBindTexture(unit, m_target, m_texture);
}
