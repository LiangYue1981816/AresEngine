#include "GfxHeader.h"


static bool InternalLoadTexture2D(CGfxTexture2D *pTexture2D, const gli::texture *texture)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (pTexture2D->Create(texture->format(), texture->extent().x, texture->extent().y, (int)texture->levels()) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTexture2D->TransferTexture2DCompressed(texture->format(), level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTexture2D->TransferTexture2D(texture->format(), level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArray(CGfxTexture2DArray *pTexture2DArray, const gli::texture *texture)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (pTexture2DArray->Create(texture->format(), texture->extent().x, texture->extent().y, (int)texture->levels(), (int)texture->layers()) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int layer = 0; layer < (int)texture->layers(); layer++) {
			for (int level = 0; level < (int)texture->levels(); level++) {
				if (pTexture2DArray->TransferTexture2DCompressed(texture->format(), layer, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}
	else {
		for (int layer = 0; layer < (int)texture->layers(); layer++) {
			for (int level = 0; level < (int)texture->levels(); level++) {
				if (pTexture2DArray->TransferTexture2D(texture->format(), layer, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArrayLayer(CGfxTexture2DArray *pTexture2DArray, const gli::texture *texture, int layer)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (gli::is_compressed(texture->format())) {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTexture2DArray->TransferTexture2DCompressed(texture->format(), layer, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTexture2DArray->TransferTexture2D(texture->format(), layer, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubeMap(CGfxTextureCubeMap *pTextureCubeMap, const gli::texture *texture)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (pTextureCubeMap->Create(texture->format(), texture->extent().x, texture->extent().y, (int)texture->levels()) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubeMapFace(CGfxTextureCubeMap *pTextureCubeMap, const gli::texture *texture, int face)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (gli::is_compressed(texture->format())) {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2DCompressed(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = 0; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2D(texture->format(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, 0, 0, texture->extent(level).x, texture->extent(level).y, format.Type, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}


bool CGfxResourceLoader::LoadTexture2D(const char *szFileName, CGfxTexture2D *pTexture2D)
{
	pTexture2D->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D) break;
			if (InternalLoadTexture2D(pTexture2D, &texture) == false) break;

			return true;
		} while (false);
	}
	pTexture2D->Destroy();
	return false;
}

bool CGfxResourceLoader::LoadTexture2DArray(const char *szFileName, CGfxTexture2DArray *pTexture2DArray)
{
	pTexture2DArray->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D_ARRAY) break;
			if (InternalLoadTexture2DArray(pTexture2DArray, &texture)) break;

			return true;
		} while (false);
	}
	pTexture2DArray->Destroy();
	return false;
}

bool CGfxResourceLoader::LoadTexture2DArrayLayer(const char *szFileName, int layer, CGfxTexture2DArray *pTexture2DArray)
{
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
		if (texture.empty()) break;
		if (texture.target() != gli::TARGET_2D) break;
		if (InternalLoadTexture2DArrayLayer(pTexture2DArray, &texture, layer) == false) break;

		return true;
	} while (false);

	return false;
}

bool CGfxResourceLoader::LoadTextureCubeMap(const char *szFileName, CGfxTextureCubeMap *pTextureCubeMap)
{
	pTextureCubeMap->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_CUBE) break;
			if (InternalLoadTextureCubeMap(pTextureCubeMap, &texture) == false) break;

			return true;
		} while (false);
	}
	pTextureCubeMap->Destroy();
	return false;
}

bool CGfxResourceLoader::LoadTextureCubeMapFace(const char *szFileName, int face, CGfxTextureCubeMap *pTextureCubeMap)
{
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		const gli::texture texture = gli::load((const char *)stream.GetAddress(), stream.GetFullSize());
		if (texture.empty()) break;
		if (texture.target() != gli::TARGET_2D) break;
		if (InternalLoadTextureCubeMapFace(pTextureCubeMap, &texture, face) == false) break;

		return true;
	} while (false);

	return false;
}
