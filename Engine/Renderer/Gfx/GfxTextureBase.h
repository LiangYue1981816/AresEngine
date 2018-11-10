#pragma once
#include "GfxRenderer.h"


class CGfxTextureBase
{
public:
	CGfxTextureBase(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxTextureBase(void)
	{

	}

public:
	uint32_t GetName(void) const
	{
		return m_name;
	}


private:
	uint32_t m_name;
};
