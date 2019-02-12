#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxSampler
{
public:
	CGfxSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
	{

	}
	virtual ~CGfxSampler(void)
	{

	}


public:
	virtual HANDLE GetSampler(void) const = 0;
};
