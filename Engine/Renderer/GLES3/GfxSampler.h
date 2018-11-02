#pragma once
#include "GfxRenderer.h"


class CGfxSampler
{
	friend class CGfxRenderer;
	friend class CGfxSamplerManager;


private:
	CGfxSampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);
	virtual ~CGfxSampler(void);


public:
	uint32_t GetSampler(void) const;


private:
	uint32_t m_sampler;
};
