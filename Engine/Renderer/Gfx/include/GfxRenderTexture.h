#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxRenderTexture : public CGfxResource
{
public:
	CGfxRenderTexture(uint32_t name)
		: m_name(name)

		, m_width(0)
		, m_height(0)
		, m_samples(0)

		, m_format(GFX_PIXELFORMAT_UNDEFINED)
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
	virtual bool Create(GfxPixelFormat pixelFormat, int width, int height, int samples = 0) = 0;
	virtual void Destroy(void) = 0;

public:
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

	GfxPixelFormat GetFormat(void) const
	{
		return m_format;
	}

	virtual uint32_t GetTarget(void) const = 0;


private:
	uint32_t m_name;

protected:
	int m_width;
	int m_height;
	int m_samples;

	GfxPixelFormat m_format;
};
