#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTexture2DArray : public CGfxTextureBase
{
	friend class CGfxRenderer;
	friend class CGfxTextureManager;


private:
	CGfxTexture2DArray(uint32_t name);
	virtual ~CGfxTexture2DArray(void);


public:
	bool Load(const char *szFileName);
	bool LoadLayer(const char *szFileName, int layer);
	bool Create(uint32_t format, uint32_t internalFormat, int width, int height, int mipLevels, int arrayLayers);
	void Destroy(void);

public:
	bool TransferTexture2DArray(const gli::texture2d_array *texture);
	bool TransferTexture2D(int layer, const gli::texture2d *texture);
	bool TransferTexture2D(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, int size, const void *data);
	bool TransferTexture2DCompressed(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, int size, const void *data);

public:
	uint32_t GetTarget(void) const;


private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;
};
