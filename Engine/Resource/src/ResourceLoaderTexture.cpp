#include "gli.hpp"
#include "GfxHeader.h"
#include "SceneHeader.h"
#include "ResourceLoader.h"


static bool InternalLoadTexture2D(CGfxTexture2D* pTexture2D, const gli::texture* texture, int baseLevel, int numLevels)
{
	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTexture2D->GetFormat() != (GfxPixelFormat)texture->format() || pTexture2D->GetWidth() < texture->extent(baseLevel).x || pTexture2D->GetHeight() < texture->extent(baseLevel).y || pTexture2D->GetLevels() < (int)texture->levels() - baseLevel) {
		if (pTexture2D->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)texture->levels() - baseLevel) == false) {
			return false;
		}
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2D->Texture2DDataCompressed((GfxPixelFormat)texture->format(), level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2D->Texture2DData((GfxPixelFormat)texture->format(), level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArray(CGfxTexture2DArray* pTexture2DArray, const gli::texture* texture, int baseLevel, int numLevels)
{
	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTexture2DArray->GetFormat() != (GfxPixelFormat)texture->format() || pTexture2DArray->GetWidth() < texture->extent(baseLevel).x || pTexture2DArray->GetHeight() < texture->extent(baseLevel).y || pTexture2DArray->GetLayers() < (int)texture->layers() || pTexture2DArray->GetLevels() < (int)texture->levels() - baseLevel) {
		if (pTexture2DArray->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)texture->layers(), (int)texture->levels() - baseLevel) == false) {
			return false;
		}
	}

	if (gli::is_compressed(texture->format())) {
		for (int layer = 0; layer < texture->layers(); layer++) {
			for (int level = baseLevel; level < baseLevel + numLevels; level++) {
				if (pTexture2DArray->Texture2DArrayDataCompressed((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}
	else {
		for (int layer = 0; layer < texture->layers(); layer++) {
			for (int level = baseLevel; level < baseLevel + numLevels; level++) {
				if (pTexture2DArray->Texture2DArrayData((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(layer, 0, level)) == false) {
					return false;
				}
			}
		}
	}

	return true;
}

static bool InternalLoadTexture2DArrayLayer(CGfxTexture2DArray* pTexture2DArray, const gli::texture* texture, int layer, int baseLevel, int numLevels)
{
	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTexture2DArray->GetFormat() != (GfxPixelFormat)texture->format() || pTexture2DArray->GetWidth() < texture->extent(baseLevel).x || pTexture2DArray->GetHeight() < texture->extent(baseLevel).y || pTexture2DArray->GetLayers() < (int)texture->layers() || pTexture2DArray->GetLevels() < (int)texture->levels() - baseLevel) {
		if (pTexture2DArray->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)texture->layers(), (int)texture->levels() - baseLevel) == false) {
			return false;
		}
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2DArray->Texture2DArrayDataCompressed((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTexture2DArray->Texture2DArrayData((GfxPixelFormat)texture->format(), layer, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubemap(CGfxTextureCubemap* pTextureCubemap, const gli::texture* texture, int baseLevel, int numLevels)
{
	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTextureCubemap->GetFormat() != (GfxPixelFormat)texture->format() || pTextureCubemap->GetWidth() < texture->extent(baseLevel).x || pTextureCubemap->GetHeight() < texture->extent(baseLevel).y || pTextureCubemap->GetLevels() < (int)texture->levels() - baseLevel) {
		if (pTextureCubemap->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)texture->levels() - baseLevel) == false) {
			return false;
		}
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_X, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 1, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 2, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Y, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 3, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), GFX_CUBEMAP_POSITIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 4, level)) == false) {
				return false;
			}
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), GFX_CUBEMAP_NEGATIVE_Z, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 5, level)) == false) {
				return false;
			}
		}
	}

	return true;
}

static bool InternalLoadTextureCubemapFace(CGfxTextureCubemap* pTextureCubemap, const gli::texture* texture, GfxCubemapFace face, int baseLevel, int numLevels)
{
	baseLevel = std::max(baseLevel, 0);
	baseLevel = std::min(baseLevel, (int)texture->levels() - 1);

	numLevels = std::max(numLevels, 1);
	numLevels = std::min(numLevels, (int)texture->levels() - baseLevel);

	if (pTextureCubemap->GetFormat() != (GfxPixelFormat)texture->format() || pTextureCubemap->GetWidth() < texture->extent(baseLevel).x || pTextureCubemap->GetHeight() < texture->extent(baseLevel).y || pTextureCubemap->GetLevels() < (int)texture->levels() - baseLevel) {
		if (pTextureCubemap->Create((GfxPixelFormat)texture->format(), texture->extent(baseLevel).x, texture->extent(baseLevel).y, (int)texture->levels() - baseLevel) == false) {
			return false;
		}
	}

	if (gli::is_compressed(texture->format())) {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubemap->TextureCubemapDataCompressed((GfxPixelFormat)texture->format(), face, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}
	else {
		for (int level = baseLevel; level < baseLevel + numLevels; level++) {
			if (pTextureCubemap->TextureCubemapData((GfxPixelFormat)texture->format(), face, level - baseLevel, 0, 0, texture->extent(level).x, texture->extent(level).y, texture->size(level), texture->data(0, 0, level)) == false) {
				return false;
			}
		}
	}

	return true;
}


bool CResourceLoader::LoadTexture2D(const char* szFileName, CGfxTexture2D* pTexture2D, int baseLevel, int numLevels)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D) return false;
	if (InternalLoadTexture2D(pTexture2D, &texture, baseLevel, numLevels) == false) return false;

	return true;
}

bool CResourceLoader::LoadTexture2DArray(const char* szFileName, CGfxTexture2DArray* pTexture2DArray, int baseLevel, int numLevels)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D_ARRAY) return false;
	if (InternalLoadTexture2DArray(pTexture2DArray, &texture, baseLevel, numLevels)) return false;

	return true;
}

bool CResourceLoader::LoadTexture2DArrayLayer(const char* szFileName, int layer, CGfxTexture2DArray* pTexture2DArray, int baseLevel, int numLevels)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D) return false;
	if (InternalLoadTexture2DArrayLayer(pTexture2DArray, &texture, layer, baseLevel, numLevels) == false) return false;

	return true;
}

bool CResourceLoader::LoadTextureCubemap(const char* szFileName, CGfxTextureCubemap* pTextureCubemap, int baseLevel, int numLevels)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_CUBE) return false;
	if (InternalLoadTextureCubemap(pTextureCubemap, &texture, baseLevel, numLevels) == false) return false;

	return true;
}

bool CResourceLoader::LoadTextureCubemapFace(const char* szFileName, GfxCubemapFace face, CGfxTextureCubemap* pTextureCubemap, int baseLevel, int numLevels)
{
	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) return false;

	const gli::texture texture = gli::load((const char*)stream.GetAddress(), stream.GetFullSize());
	if (texture.empty()) return false;
	if (texture.target() != gli::TARGET_2D) return false;
	if (InternalLoadTextureCubemapFace(pTextureCubemap, &texture, face, baseLevel, numLevels) == false) return false;

	return true;
}
