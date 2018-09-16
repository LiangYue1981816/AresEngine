#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTexture2DArray.h"


CGfxTexture2DArray::CGfxTexture2DArray(GLuint name)
	: CGfxTextureBase(name)
{

}

CGfxTexture2DArray::~CGfxTexture2DArray(void)
{

}

bool CGfxTexture2DArray::Load(const char *szFileName)
{
	try {
		Free();

		char szFullPath[260];
		Renderer()->GetTextureFullPath(szFileName, szFullPath);

		const gli::texture texture = gli::load(szFullPath);
		if (texture.empty()) throw 0;

		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		if (texture.target() != gli::TARGET_2D_ARRAY) throw 1;
		if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, texture.levels(), texture.layers()) == false) throw 2;
		if (TransferTexture2DArray((const gli::texture2d_array *)&texture) == false) throw 3;

		return true;
	}
	catch (int) {
		Free();
		return false;
	}
}

bool CGfxTexture2DArray::Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLuint mipLevels, GLuint arrayLayers)
{
	Free();

	m_format = format;
	m_internalFormat = internalFormat;

	m_width = width;
	m_height = height;

	m_mipLevels = mipLevels;
	m_arrayLayers = arrayLayers;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_mipLevels, m_internalFormat, m_width, m_height, m_arrayLayers);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2DArray(const gli::texture2d_array *texture)
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

	if (texture->target() != gli::TARGET_2D_ARRAY) {
		return false;
	}

	if (m_format != format.External) {
		return false;
	}

	if (m_internalFormat != format.Internal) {
		return false;
	}

	if (m_mipLevels != texture->levels()) {
		return false;
	}

	if (m_arrayLayers != texture->layers()) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		for (int layer = 0; layer < (int)texture->layers(); layer++) {
			for (int level = 0; level < (int)texture->levels(); level++) {
				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.Internal, texture->size(level), texture->data(layer, 0, level));
				}
				else {
					glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.External, format.Type, texture->data(layer, 0, level));
				}
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2D(GLuint layer, const gli::texture2d *texture)
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

	if (m_mipLevels < texture->levels()) {
		return false;
	}

	if (m_arrayLayers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (gli::is_compressed(texture->format())) {
				glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.Internal, texture->size(level), texture->data(layer, 0, level));
			}
			else {
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.External, format.Type, texture->data(layer, 0, level));
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2D(GLuint layer, GLuint level, GLenum format, GLsizei width, GLsizei height, GLenum type, const GLvoid *data)
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

	if (m_arrayLayers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, width, height, 1, format, type, data);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2DCompressed(GLuint layer, GLuint level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data)
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

	if (m_arrayLayers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, width, height, 1, format, size, data);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}
