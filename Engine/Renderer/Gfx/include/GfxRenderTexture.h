#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxRenderTexture : public CGfxResource
{
public:
	CGfxRenderTexture(uint32_t name)
		: m_name(name)

		, m_format(GFX_PIXELFORMAT_UNDEFINED)
		, m_type(GFX_TEXTURE_INVALID_ENUM)

		, m_width(0)
		, m_height(0)
		, m_samples(0)
	{

	}
	virtual ~CGfxRenderTexture(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int samples = 1) = 0;
	virtual void Destroy(void) = 0;

public:
	GfxPixelFormat GetFormat(void) const
	{
		return m_format;
	}

	GfxTextureType GetTarget(void) const
	{
		return m_type;
	}

	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}

	int GetSamples(void) const
	{
		return m_samples;
	}


private:
	uint32_t m_name;

protected:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_samples;
};
