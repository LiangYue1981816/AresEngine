#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxTextureBase : public CGfxResource
{
public:
	CGfxTextureBase(uint32_t name)
		: m_name(name)

		, m_width(0)
		, m_height(0)
		, m_levels(0)
	{

	}
	virtual ~CGfxTextureBase(void)
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

	virtual bool IsExtern(void) const = 0;

	virtual uint32_t GetTarget(void) const = 0;
	virtual uint32_t GetTexture(void) const = 0;

	virtual GfxPixelFormat GetFormat(void) const = 0;


private:
	uint32_t m_name;

protected:
	int m_width;
	int m_height;
	int m_levels;
};
