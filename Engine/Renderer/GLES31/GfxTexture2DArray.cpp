#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTexture2DArray.h"


CGfxTexture2DArray::CGfxTexture2DArray(uint32_t name)
	: CGfxTextureBase(name)
{

}

CGfxTexture2DArray::~CGfxTexture2DArray(void)
{
	Destroy();
}

bool CGfxTexture2DArray::Load(const char *szFileName)
{
	try {
		Destroy();

		const gli::texture texture = gli::load(Renderer()->GetResourceFullName(szFileName));
		if (texture.empty()) throw 0;
		if (texture.target() != gli::TARGET_2D_ARRAY) throw 1;

		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, (int)texture.levels(), (int)texture.layers()) == false) throw 2;
		if (TransferTexture2DArray((const gli::texture2d_array *)&texture) == false) throw 3;

		return true;
	}
	catch (int) {
		Destroy();
		return false;
	}
}

bool CGfxTexture2DArray::LoadLayer(const char *szFileName, int layer)
{
	const gli::texture texture = gli::load(Renderer()->GetResourceFullName(szFileName));
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D) return false;

	return TransferTexture2D(layer, (const gli::texture2d *)&texture);
}

bool CGfxTexture2DArray::Create(uint32_t format, uint32_t internalFormat, int width, int height, int mipLevels, int arrayLayers)
{
	Destroy();

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

void CGfxTexture2DArray::Destroy(void)
{
	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	CGfxTextureBase::Destroy();
}

bool CGfxTexture2DArray::TransferTexture2DArray(const gli::texture2d_array *texture)
{
	if (texture == NULL) {
		return false;
	}

	if (texture->target() != gli::TARGET_2D_ARRAY) {
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

	if (m_arrayLayers != (int)texture->layers()) {
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

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		for (int layer = 0; layer < (int)texture->layers(); layer++) {
			for (int level = 0; level < (int)texture->levels(); level++) {
				CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
				{
					m_size[layer][level] = (uint32_t)texture->size(level);

					if (gli::is_compressed(texture->format())) {
						glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.Internal, (int)texture->size(level), texture->data(layer, 0, level));
					}
					else {
						glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.External, format.Type, texture->data(layer, 0, level));
					}
				}
				CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2D(int layer, const gli::texture2d *texture)
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

	if (m_mipLevels < (int)texture->levels()) {
		return false;
	}

	if (m_arrayLayers < (int)layer) {
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

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
			{
				m_size[layer][level] = (uint32_t)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.Internal, (int)texture->size(level), texture->data(layer, 0, level));
				}
				else {
					glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, texture->extent(level).x, texture->extent(level).y, 1, format.External, format.Type, texture->data(layer, 0, level));
				}
			}
			CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
		}
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2D(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, int size, const void *data)
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

	if (m_arrayLayers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = (uint32_t)size;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGfxTexture2DArray::TransferTexture2DCompressed(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, int size, const void *data)
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

	if (m_arrayLayers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = (uint32_t)size;
			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

uint32_t CGfxTexture2DArray::GetTarget(void) const
{
	return GL_TEXTURE_2D_ARRAY;
}
