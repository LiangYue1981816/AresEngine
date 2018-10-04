#pragma once
#include "GfxRenderer.h"


class CGfxSamplerManager
{
	friend class CGfxRenderer;


private:
	CGfxSamplerManager(void);
	virtual ~CGfxSamplerManager(void);


private:
	CGfxSampler* CreateSampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);


private:
	eastl::unordered_map<uint32_t, CGfxSampler*> m_pSamplers;
};
