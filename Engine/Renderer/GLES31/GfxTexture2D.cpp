#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTexture2D.h"


CGfxTexture2D::CGfxTexture2D(uint32_t name)
	: CGfxTextureBase(name)
{

}

CGfxTexture2D::~CGfxTexture2D(void)
{
	Destroy();
}

bool CGfxTexture2D::Load(const char *szFileName)
{
	try {
		Destroy();

		const gli::texture texture = gli::load(Renderer()->GetResourceFullName(szFileName));
		if (texture.empty()) throw 0;
		if (texture.target() != gli::TARGET_2D) throw 1;

		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, (int)texture.levels()) == false) throw 2;
		if (TransferTexture2D((const gli::texture2d *)&texture) == false) throw 3;

		return true;
	}
	catch (int) {
		Destroy();
		return false;
	}
}

bool CGfxTexture2D::Create(uint32_t format, uint32_t internalFormat, int width, int height, int mipLevels, int samples)
{
	Destroy();

	m_format = format;
	m_internalFormat = internalFormat;

	m_width = width;
	m_height = height;

	m_samples = samples;
	m_mipLevels = mipLevels;

	if (m_samples == 0) {
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, m_mipLevels, m_internalFormat, m_width, m_height);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, m_internalFormat, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}

	return true;
}

void CGfxTexture2D::Destroy(void)
{
	for (const auto &itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}

	CGfxTextureBase::Destroy();
}

bool CGfxTexture2D::TransferTexture2D(const gli::texture2d *texture)
{
	if (texture == NULL) {
		return false;
	}

	if (texture->target() != gli::TARGET_2D) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels != (int)texture->levels()) {
		return false;
	}

	if (m_samples != 0) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (format.External != m_format) {
		return false;
	}

	if (format.Internal != m_internalFormat) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			CGfxProfiler::DecTextureDataSize(m_size[level]);
			{
				m_size[level] = (uint32_t)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 0, level));
				}
				else {
					glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 0, level));
				}
			}
			CGfxProfiler::IncTextureDataSize(m_size[level]);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool CGfxTexture2D::TransferTexture2D(int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, int size, const void *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels < level) {
		return false;
	}

	if (m_samples != 0) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = (uint32_t)size;
			glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[level]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool CGfxTexture2D::TransferTexture2DCompressed(int level, uint32_t format, int xoffset, int yoffset, int width, int height, int size, const void *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels < level) {
		return false;
	}

	if (m_samples != 0) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = (uint32_t)size;
			glCompressedTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[level]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

uint32_t CGfxTexture2D::GetTarget(void) const
{
	return m_samples == 0 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
}
