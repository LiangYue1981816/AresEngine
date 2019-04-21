#include "ResourceLoader.h"

#include "Stream.h"
#include "FileManager.h"


static bool InternalLoadTexture2D(CGfxTexture2D* pTexture2D, const gli::texture* texture, uint32_t baseLevel)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (baseLevel > texture->levels() - 1) {
		baseLevel = texture->levels() - 1;
	}

	if (pTexture2D->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)(texture->levels() - baseLevel)) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTexture2D->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTexture2D->TransferTexture2D((GfxPixelFormat)texture->format(), level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArray(CGfxTexture2DArray* pTexture2DArray, const gli::texture* texture, uint32_t baseLevel)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (baseLevel > texture->levels() - 1) {
		baseLevel = texture->levels() - 1;
	}

	if (pTexture2DArray->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)(texture->levels() - baseLevel), (int)texture->layers()) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int layer = 0; layer < (int)texture->layers(); layer++) {
			for (int level = baseLevel; level < (int)texture->levels(); level++) {
				if (pTexture2DArray->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}
	else {
		for (int layer = 0; layer < (int)texture->layers(); layer++) {
			for (int level = baseLevel; level < (int)texture->levels(); level++) {
				if (pTexture2DArray->TransferTexture2D((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArrayLayer(CGfxTexture2DArray* pTexture2DArray, const gli::texture* texture, int layer, uint32_t baseLevel)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (baseLevel > texture->levels() - 1) {
		baseLevel = texture->levels() - 1;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTexture2DArray->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTexture2DArray->TransferTexture2D((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubeMap(CGfxTextureCubeMap* pTextureCubeMap, const gli::texture* texture, uint32_t baseLevel)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (baseLevel > texture->levels() - 1) {
		baseLevel = texture->levels() - 1;
	}

	if (pTextureCubeMap->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)(texture->levels() - baseLevel)) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_POSITIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_NEGATIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_POSITIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_NEGATIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_POSITIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_NEGATIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_POSITIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_NEGATIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_POSITIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_NEGATIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_POSITIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_TEXTURE_CUBEMAP_NEGATIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubeMapFace(CGfxTextureCubeMap* pTextureCubeMap, const gli::texture* texture, GfxTextureCubeMapFace face, uint32_t baseLevel)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	if (baseLevel > texture->levels() - 1) {
		baseLevel = texture->levels() - 1;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), face, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < (int)texture->levels(); level++) {
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), face, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, GFX_DATA_UNSIGNED_BYTE, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}


bool CResourceLoader::LoadTexture2D(const char* szFileName, CGfxTexture2D* pTexture2D, uint32_t baseLevel)
{
	pTexture2D->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D) break;
			if (InternalLoadTexture2D(pTexture2D, &texture, baseLevel) == false) break;

			return true;
		} while (false);
	}
	pTexture2D->Destroy();
	return false;
}

bool CResourceLoader::LoadTexture2DArray(const char* szFileName, CGfxTexture2DArray* pTexture2DArray, uint32_t baseLevel)
{
	pTexture2DArray->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D_ARRAY) break;
			if (InternalLoadTexture2DArray(pTexture2DArray, &texture, baseLevel)) break;

			return true;
		} while (false);
	}
	pTexture2DArray->Destroy();
	return false;
}

bool CResourceLoader::LoadTexture2DArrayLayer(const char* szFileName, int layer, CGfxTexture2DArray* pTexture2DArray, uint32_t baseLevel)
{
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
		if (texture.empty()) break;
		if (texture.target() != gli::TARGET_2D) break;
		if (InternalLoadTexture2DArrayLayer(pTexture2DArray, &texture, layer, baseLevel) == false) break;

		return true;
	} while (false);

	return false;
}

bool CResourceLoader::LoadTextureCubeMap(const char* szFileName, CGfxTextureCubeMap* pTextureCubeMap, uint32_t baseLevel)
{
	pTextureCubeMap->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_CUBE) break;
			if (InternalLoadTextureCubeMap(pTextureCubeMap, &texture, baseLevel) == false) break;

			return true;
		} while (false);
	}
	pTextureCubeMap->Destroy();
	return false;
}

bool CResourceLoader::LoadTextureCubeMapFace(const char* szFileName, GfxTextureCubeMapFace face, CGfxTextureCubeMap* pTextureCubeMap, uint32_t baseLevel)
{
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
		if (texture.empty()) break;
		if (texture.target() != gli::TARGET_2D) break;
		if (InternalLoadTextureCubeMapFace(pTextureCubeMap, &texture, face, baseLevel) == false) break;

		return true;
	} while (false);

	return false;
}
