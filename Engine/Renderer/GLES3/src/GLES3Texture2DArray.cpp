#include "GLES3Renderer.h"


CGLES3Texture2DArray::CGLES3Texture2DArray(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3TextureBase(pManager, name)
{

}

CGLES3Texture2DArray::~CGLES3Texture2DArray(void)
{
	Destroy();
}

void CGLES3Texture2DArray::Release(void)
{
	m_pManager->DestroyTexture(this);
}

bool CGLES3Texture2DArray::Load(const char *szFileName)
{
	Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D_ARRAY) break;

			gli::gl GL(gli::gl::PROFILE_ES30);
			gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

			if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, (int)texture.levels(), (int)texture.layers()) == false) break;
			if (TransferTexture2DArray((const gli::texture2d_array *)&texture) == false) break;

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

bool CGLES3Texture2DArray::LoadLayer(const char *szFileName, int layer)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D) return false;

	return TransferTexture2D(layer, (const gli::texture2d *)&texture);
}

bool CGLES3Texture2DArray::Create(uint32_t format, uint32_t internalFormat, int width, int height, int levels, int layers)
{
	Destroy();

	m_format = format;
	m_internalFormat = internalFormat;

	m_width = width;
	m_height = height;

	m_levels = levels;
	m_layers = layers;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_levels, m_internalFormat, m_width, m_height, m_layers);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

void CGLES3Texture2DArray::Destroy(void)
{
	CGLES3TextureBase::Destroy();

	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();
}

bool CGLES3Texture2DArray::TransferTexture2DArray(const gli::texture2d_array *texture)
{
	if (texture == nullptr) {
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

	if (m_levels != texture->levels()) {
		return false;
	}

	if (m_layers != texture->layers()) {
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

bool CGLES3Texture2DArray::TransferTexture2D(int layer, const gli::texture2d *texture)
{
	if (texture == nullptr) {
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

	if (m_levels != texture->levels()) {
		return false;
	}

	if (m_layers < layer) {
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

bool CGLES3Texture2DArray::TransferTexture2D(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == GL_TRUE) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = size;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool CGLES3Texture2DArray::TransferTexture2DCompressed(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	if (m_texture == 0) {
		return false;
	}

	if (m_bExtern == GL_TRUE) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
		{
			m_size[layer][level] = size;
			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, layer, width, height, 1, format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[layer][level]);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

uint32_t CGLES3Texture2DArray::GetTarget(void) const
{
	return GL_TEXTURE_2D_ARRAY;
}
