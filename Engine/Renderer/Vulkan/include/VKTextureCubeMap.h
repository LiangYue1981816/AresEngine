#pragma once
#include "VKRenderer.h"


class CVKTextureCubeMap : public CVKTexture, public CGfxTextureCubeMap
{
	friend class CVKDevice;
	friend class CVKTextureManager;


private:
	CVKTextureCubeMap(CVKDevice *pDevice, CVKTextureManager *pManager, uint32_t name);
	virtual ~CVKTextureCubeMap(void);
	virtual void Release(void);


public:
	bool Create(uint64_t texture);
	bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels);
	void Destroy(void);

public:
	bool TransferTexture2D(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);


private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;
};
