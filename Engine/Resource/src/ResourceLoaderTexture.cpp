#include "ResourceLoader.h"

#include "Stream.h"
#include "FileManager.h"


static bool InternalLoadTexture2D(CGfxTexture2D* pTexture2D, const gli::texture* texture, int baseLevel, int numLevels)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTexture2D->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, numLevels) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2D->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2D->TransferTexture2D((GfxPixelFormat)texture->format(), level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArray(CGfxTexture2DArray* pTexture2DArray, const gli::texture* texture, int baseLevel, int numLevels)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTexture2DArray->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)texture->layers(), numLevels) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int layer = 0; layer < texture->layers(); layer++) {
			for (int level = baseLevel; level < baseLevel + numLevels; level++) {
				if (pTexture2DArray->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}
	else {
		for (int layer = 0; layer < texture->layers(); layer++) {
			for (int level = baseLevel; level < baseLevel + numLevels; level++) {
				if (pTexture2DArray->TransferTexture2D((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArrayLayer(CGfxTexture2DArray* pTexture2DArray, const gli::texture* texture, int layer, int baseLevel, int numLevels)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2DArray->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2DArray->TransferTexture2D((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubeMap(CGfxTextureCubeMap* pTextureCubeMap, const gli::texture* texture, int baseLevel, int numLevels)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTextureCubeMap->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, numLevels) == false) {
		return false;
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubeMapFace(CGfxTextureCubeMap* pTextureCubeMap, const gli::texture* texture, GfxCubeMapFace face, int baseLevel, int numLevels)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format format = GL.translate(texture->format(), texture->swizzles());

	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubeMap->TransferTexture2DCompressed((GfxPixelFormat)texture->format(), face, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubeMap->TransferTexture2D((GfxPixelFormat)texture->format(), face, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}


bool CResourceLoader::LoadTexture2D(const char* szFileName, CGfxTexture2D* pTexture2D, int baseLevel, int numLevels)
{
	pTexture2D->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D) break;
			if (InternalLoadTexture2D(pTexture2D, &texture, baseLevel, numLevels) == false) break;

			return true;
		} while (false);
	}
	pTexture2D->Destroy();
	return false;
}

bool CResourceLoader::LoadTexture2DArray(const char* szFileName, CGfxTexture2DArray* pTexture2DArray, int baseLevel, int numLevels)
{
	pTexture2DArray->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_2D_ARRAY) break;
			if (InternalLoadTexture2DArray(pTexture2DArray, &texture, baseLevel, numLevels)) break;

			return true;
		} while (false);
	}
	pTexture2DArray->Destroy();
	return false;
}

bool CResourceLoader::LoadTexture2DArrayLayer(const char* szFileName, int layer, CGfxTexture2DArray* pTexture2DArray, int baseLevel, int numLevels)
{
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
		if (texture.empty()) break;
		if (texture.target() != gli::TARGET_2D) break;
		if (InternalLoadTexture2DArrayLayer(pTexture2DArray, &texture, layer, baseLevel, numLevels) == false) break;

		return true;
	} while (false);

	return false;
}

bool CResourceLoader::LoadTextureCubeMap(const char* szFileName, CGfxTextureCubeMap* pTextureCubeMap, int baseLevel, int numLevels)
{
	pTextureCubeMap->Destroy();
	{
		do {
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) break;

			const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
			if (texture.empty()) break;
			if (texture.target() != gli::TARGET_CUBE) break;
			if (InternalLoadTextureCubeMap(pTextureCubeMap, &texture, baseLevel, numLevels) == false) break;

			return true;
		} while (false);
	}
	pTextureCubeMap->Destroy();
	return false;
}

bool CResourceLoader::LoadTextureCubeMapFace(const char* szFileName, GfxCubeMapFace face, CGfxTextureCubeMap* pTextureCubeMap, int baseLevel, int numLevels)
{
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
		if (texture.empty()) break;
		if (texture.target() != gli::TARGET_2D) break;
		if (InternalLoadTextureCubeMapFace(pTextureCubeMap, &texture, face, baseLevel, numLevels) == false) break;

		return true;
	} while (false);

	return false;
}
