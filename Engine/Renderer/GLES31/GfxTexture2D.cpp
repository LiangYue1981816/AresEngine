#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTexture2D.h"


CGfxTexture2D::CGfxTexture2D(GLuint name)
	: CGfxTextureBase(name)
{

}

CGfxTexture2D::~CGfxTexture2D(void)
{
	Free();
}

void CGfxTexture2D::Free(void)
{
	for (const auto &itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}

	CGfxTextureBase::Free();
}

bool CGfxTexture2D::Load(const char *szFileName)
{
	try {
		Free();

		char szFullPath[260];
		Renderer()->GetTextureFullPath(szFileName, szFullPath);

		const gli::texture texture = gli::load(szFullPath);
		if (texture.empty()) throw 0;

		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		if (texture.target() != gli::TARGET_2D) throw 1;
		if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, (GLsizei)texture.levels()) == false) throw 2;
		if (TransferTexture2D((const gli::texture2d *)&texture) == false) throw 3;

		return true;
	}
	catch (int) {
		Free();
		return false;
	}
}

bool CGfxTexture2D::Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels)
{
	Free();

	m_format = format;
	m_internalFormat = internalFormat;

	m_width = width;
	m_height = height;

	m_mipLevels = mipLevels;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexStorage2D(GL_TEXTURE_2D, m_mipLevels, m_internalFormat, m_width, m_height);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool CGfxTexture2D::TransferTexture2D(const gli::texture2d *texture)
{
	if (texture == NULL) {
		return false;
	}

	if (m_texture == 0) {
		return false;
	}

	if (m_extern == GL_TRUE) {
		return false;
	}

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (texture->target() != gli::TARGET_2D) {
		return false;
	}

	if (m_format != format.External) {
		return false;
	}

	if (m_internalFormat != format.Internal) {
		return false;
	}

	if (m_mipLevels != (GLsizei)texture->levels()) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			CGfxProfiler::DecTextureDataSize(m_size[level]);
			{
				m_size[level] = (GLsizeiptr)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 0, level));
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

bool CGfxTexture2D::TransferTexture2D(GLsizei level, GLenum format, GLsizei width, GLsizei height, GLenum type, GLsizei size, const GLvoid *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_extern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels < level) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = (GLsizeiptr)size;
			glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[level]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool CGfxTexture2D::TransferTexture2DCompressed(GLsizei level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_extern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels < level) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		{
			m_size[level] = (GLsizeiptr)size;
			glCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[level]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
