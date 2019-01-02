#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTexture : public CGfxResource
{
public:
	CGfxTexture(uint32_t name)
		: m_name(name)

		, m_format(GFX_PIXELFORMAT_UNDEFINED)
		, m_type(GFX_TEXTURE_INVALID_ENUM)

		, m_width(0)
		, m_height(0)
		, m_levels(0)
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
	GfxPixelFormat GetFormat(void) const
	{
		return m_format;
	}

	GfxTextureType GetType(void) const
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

	int GetLevels(void) const
	{
		return m_levels;
	}


private:
	uint32_t m_name;

protected:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_levels;
};
