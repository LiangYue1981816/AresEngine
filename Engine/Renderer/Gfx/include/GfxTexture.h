#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture : public CGfxResource
{
public:
	CGfxTexture(uint32_t name)
		: m_name(name)

		, m_width(0)
		, m_height(0)
		, m_levels(0)

		, m_format(GFX_PIXELFORMAT_UNDEFINED)
	{

	}
	virtual ~CGfxTexture(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}

	int GetLevels(void) const
	{
		return m_levels;
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
	int m_levels;

	GfxPixelFormat m_format;
};
