#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2DArray : public CGfxResource
{
protected:
	CGfxTexture2DArray(void)
		: m_width(0)
		, m_height(0)
		, m_layers(0)
	{

	}
	virtual ~CGfxTexture2DArray(void)
	{

	}


public:
	virtual bool Load(const char *szFileName) = 0;
	virtual bool LoadLayer(const char *szFileName, int layer) = 0;
	virtual bool Create(uint32_t format, uint32_t internalFormat, int width, int height, int levels, int layers) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2DArray(const gli::texture2d_array *texture) = 0;
	virtual bool TransferTexture2D(int layer, const gli::texture2d *texture) = 0;
	virtual bool TransferTexture2D(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(int layer, int level, uint32_t format, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;

public:
	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}

	int GetLayers(void) const
	{
		return m_layers;
	}


protected:
	int m_width;
	int m_height;
	int m_layers;
};
