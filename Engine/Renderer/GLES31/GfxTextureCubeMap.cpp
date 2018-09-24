#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTextureCubeMap.h"


CGfxTextureCubeMap::CGfxTextureCubeMap(GLuint name)
	: CGfxTextureBase(name)
{

}

CGfxTextureCubeMap::~CGfxTextureCubeMap(void)
{
	Free();
}

void CGfxTextureCubeMap::Free(void)
{
	for (const auto &itFaceSize : m_size) {
		for (const auto &itLevelSize : itFaceSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	CGfxTextureBase::Free();
}

bool CGfxTextureCubeMap::Load(const char *szFileName)
{
	try {
		Free();

		char szFullPath[260];
		Renderer()->GetTextureFullPath(szFileName, szFullPath);

		const gli::texture texture = gli::load(szFullPath);
		if (texture.empty()) throw 0;

		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		if (texture.target() != gli::TARGET_CUBE) throw 1;
		if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, (GLsizei)texture.levels()) == false) throw 2;
		if (TransferTextureCubeMap((const gli::texture_cube *)&texture) == false) throw 3;

		return true;
	}
	catch (int) {
		Free();
		return false;
	}
}

bool CGfxTextureCubeMap::LoadFace(const char *szFileName, GLsizei face)
{
	char szFullPath[260];
	Renderer()->GetTextureFullPath(szFileName, szFullPath);

	const gli::texture texture = gli::load(szFullPath);
	if (texture.empty()) return false;

	return TransferTexture2D(face, (const gli::texture2d *)&texture);
}

bool CGfxTextureCubeMap::Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels)
{
	Free();

	m_format = format;
	m_internalFormat = internalFormat;

	m_width = width;
	m_height = height;

	m_mipLevels = mipLevels;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_mipLevels, m_internalFormat, m_width, m_height);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGfxTextureCubeMap::TransferTextureCubeMap(const gli::texture_cube *texture)
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

	if (texture->target() != gli::TARGET_CUBE) {
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X][level]);
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_X][level]);
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Y][level]);
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y][level]);
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Z][level]);
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z][level]);
			{
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X][level] = (GLsizeiptr)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_X][level] = (GLsizeiptr)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Y][level] = (GLsizeiptr)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y][level] = (GLsizeiptr)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Z][level] = (GLsizeiptr)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z][level] = (GLsizeiptr)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 0, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 1, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 2, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 3, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 4, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 5, level));
				}
				else {
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 0, level));
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 1, level));
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 2, level));
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 3, level));
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 4, level));
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 5, level));
				}
			}
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X][level]);
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_X][level]);
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Y][level]);
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y][level]);
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Z][level]);
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z][level]);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGfxTextureCubeMap::TransferTexture2D(GLsizei face, const gli::texture2d *texture)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X + face][level]);
			{
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X + face][level] = (GLsizeiptr)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (GLsizei)texture->size(level), texture->data(0, 0, level));
				}
				else {
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.External, format.Type, texture->data(0, 0, level));
				}
			}
			CGfxProfiler::IncTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X + face][level]);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGfxTextureCubeMap::TransferTexture2D(GLsizei face, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLenum type, GLsizei size, const GLvoid *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_extern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels != level) {
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = (GLsizeiptr)size;
			glTexSubImage2D(face, level, 0, 0, width, height, format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGfxTextureCubeMap::TransferTexture2DCompressed(GLsizei face, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_extern == GL_TRUE) {
		return false;
	}

	if (m_mipLevels != level) {
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = (GLsizeiptr)size;
			glCompressedTexSubImage2D(face, level, 0, 0, width, height, format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}
