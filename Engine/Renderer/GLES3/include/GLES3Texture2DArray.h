#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DArray : public CGLES3Texture, public CGfxTexture2DArray
{
	friend class CGLES3TextureManager;


private:
	CGLES3Texture2DArray(CGLES3TextureManager *pManager, uint32_t name);
	virtual ~CGLES3Texture2DArray(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	GfxPixelFormat GetFormat(void) const;
	GfxTextureType GetType(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLevels(void) const;
	int GetLayers(void) const;

public:
	bool Create(uint64_t texture);
	bool Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int layers);
	void Destroy(void);

public:
	bool TransferTexture2D(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);


private:
	uint32_t m_name;

private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_levels;
	int m_layers;

private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;
};
