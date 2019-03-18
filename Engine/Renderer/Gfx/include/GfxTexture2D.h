#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture2D : public CGfxResource
{
public:
	CGfxTexture2D(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxTexture2D(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual HANDLE GetTexture(void) const = 0;

	virtual GfxPixelFormat GetFormat(void) const = 0;
	virtual GfxTextureType GetType(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetLevels(void) const = 0;
	virtual int GetSamples(void) const = 0;

public:
	virtual bool Create(GfxPixelFormat format, int width, int height, int levels, int samples = 1) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool TransferTexture2D(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data) = 0;
	virtual bool TransferTexture2DCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data) = 0;


private:
	uint32_t m_name;
};
