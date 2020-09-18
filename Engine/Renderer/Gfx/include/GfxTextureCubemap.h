#pragma once
#include "PreHeader.h"


class CGfxTextureCubemap : public CGfxResource
{
public:
	CGfxTextureCubemap(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxTextureCubemap(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual GfxPixelFormat GetFormat(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetLevels(void) const = 0;


private:
	uint32_t m_name;
};
