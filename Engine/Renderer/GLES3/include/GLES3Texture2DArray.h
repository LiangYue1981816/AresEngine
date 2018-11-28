#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DArray : public CGLES3TextureBase, public CGfxTexture2DArray
{
	friend class CGLES3TextureManager;


private:
	CGLES3Texture2DArray(CGLES3TextureManager *pManager, uint32_t name);
	virtual ~CGLES3Texture2DArray(void);
	virtual void Release(void);


public:
	bool Create(uint32_t texture);
	bool Create(uint32_t format, int width, int height, int levels, int layers);
	void Destroy(void);

public:
	bool TransferTexture2D(uint32_t format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(uint32_t format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);

public:
	bool IsExtern(void) const;

	uint32_t GetTarget(void) const;
	uint32_t GetTexture(void) const;

	uint32_t GetFormat(void) const;


private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;
};
