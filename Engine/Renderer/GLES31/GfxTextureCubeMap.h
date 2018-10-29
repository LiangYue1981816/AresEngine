#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTextureCubeMap : public CGfxTextureBase
{
	friend class CGfxRenderer;
	friend class CGfxTextureManager;


private:
	CGfxTextureCubeMap(uint32_t name);
	virtual ~CGfxTextureCubeMap(void);


public:
	bool Load(const char *szFileName);
	bool LoadFace(const char *szFileName, int face);
	bool Create(uint32_t format, uint32_t internalFormat, int width, int height, int mipLevels);
	void Destroy(void);

public:
	bool TransferTextureCubeMap(const gli::texture_cube *texture);
	bool TransferTexture2D(int face, const gli::texture2d *texture);
	bool TransferTexture2D(int face, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(int face, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);

public:
	uint32_t GetTarget(void) const;


private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;
};
