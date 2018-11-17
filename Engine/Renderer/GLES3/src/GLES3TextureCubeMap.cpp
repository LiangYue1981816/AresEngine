#include "GLES3Renderer.h"


CGLES3TextureCubeMap::CGLES3TextureCubeMap(CGLES3TextureManager *pManager, uint32_t name)
	: CGLES3TextureBase(pManager, name)
{

}

CGLES3TextureCubeMap::~CGLES3TextureCubeMap(void)
{
	Destroy();
}

void CGLES3TextureCubeMap::Release(void)
{
	m_pManager->DestroyTexture(this);
}

bool CGLES3TextureCubeMap::Load(const char *szFileName)
{
	Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_CUBE) break;

			gli::gl GL(gli::gl::PROFILE_ES30);
			gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

			if (Create(format.External, format.Internal, texture.extent().x, texture.extent().y, (int)texture.levels()) == false) break;
			if (TransferTextureCubeMap((const gli::texture_cube *)&texture) == false) break;

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

bool CGLES3TextureCubeMap::LoadFace(const char *szFileName, int face)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D) return false;

	return TransferTexture2D(face, (const gli::texture2d *)&texture);
}

bool CGLES3TextureCubeMap::Create(uint32_t format, uint32_t internalFormat, int width, int height, int levels)
{
	Destroy();

	m_format = format;
	m_internalFormat = internalFormat;

	m_width = width;
	m_height = height;

	m_levels = levels;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_levels, m_internalFormat, m_width, m_height);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

void CGLES3TextureCubeMap::Destroy(void)
{
	CGLES3TextureBase::Destroy();

	for (const auto &itFaceSize : m_size) {
		for (const auto &itLevelSize : itFaceSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();
}

bool CGLES3TextureCubeMap::TransferTextureCubeMap(const gli::texture_cube *texture)
{
	if (texture == nullptr) {
		return false;
	}

	if (texture->target() != gli::TARGET_CUBE) {
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

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (format.External != m_format) {
		return false;
	}

	if (format.Internal != m_internalFormat) {
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
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X][level] = (uint32_t)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_X][level] = (uint32_t)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Y][level] = (uint32_t)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y][level] = (uint32_t)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_Z][level] = (uint32_t)texture->size(level);
				m_size[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z][level] = (uint32_t)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 0, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 1, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 2, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 3, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 4, level));
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 5, level));
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

bool CGLES3TextureCubeMap::TransferTexture2D(int face, const gli::texture2d *texture)
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

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (format.External != m_format) {
		return false;
	}

	if (format.Internal != m_internalFormat) {
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		for (int level = 0; level < (int)texture->levels(); level++) {
			CGfxProfiler::DecTextureDataSize(m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X + face][level]);
			{
				m_size[GL_TEXTURE_CUBE_MAP_POSITIVE_X + face][level] = (uint32_t)texture->size(level);

				if (gli::is_compressed(texture->format())) {
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Internal, (int)texture->size(level), texture->data(0, 0, level));
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

bool CGLES3TextureCubeMap::TransferTexture2D(int face, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = size;
			glTexSubImage2D(face, level, xoffset, yoffset, width, height, format, type, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

bool CGLES3TextureCubeMap::TransferTexture2DCompressed(int face, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	{
		CGfxProfiler::DecTextureDataSize(m_size[face][level]);
		{
			m_size[face][level] = size;
			glCompressedTexSubImage2D(face, level, xoffset, yoffset, width, height, format, size, data);
		}
		CGfxProfiler::IncTextureDataSize(m_size[face][level]);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

uint32_t CGLES3TextureCubeMap::GetTarget(void) const
{
	return GL_TEXTURE_CUBE_MAP;
}
