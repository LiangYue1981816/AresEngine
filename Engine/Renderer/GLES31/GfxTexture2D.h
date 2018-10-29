#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTexture2D : public CGfxTextureBase
{
	friend class CGfxRenderer;
	friend class CGfxTextureManager;


private:
	CGfxTexture2D(uint32_t name);
	virtual ~CGfxTexture2D(void);


public:
	bool Load(const char *szFileName);
	bool Create(uint32_t format, uint32_t internalFormat, int width, int height, int mipLevels, int samples = 0);
	void Destroy(void);

public:
	bool TransferTexture2D(const gli::texture2d *texture);
	bool TransferTexture2D(int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);

public:
	uint32_t GetTarget(void) const;


private:
	eastl::unordered_map<int, uint32_t> m_size;
};
