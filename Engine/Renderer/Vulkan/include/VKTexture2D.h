#pragma once
#include "VKRenderer.h"


class CVKTexture2D : public CGfxTexture2D
{
	friend class CVKDevice;
	friend class CVKTextureManager;


private:
	CVKTexture2D(CVKDevice *pDevice, CVKTextureManager *pManager, uint32_t name);
	virtual ~CVKTexture2D(void);
	virtual void Release(void);


public:
	uint32_t GetTarget(void) const;

public:
	bool Create(uint32_t texture);
	bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples = 0);
	void Destroy(void);

public:
	bool TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);


private:
	eastl::unordered_map<int, uint32_t> m_size;
};
