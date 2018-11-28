#pragma once
#include "GLES3Renderer.h"


class CGLES3SamplerManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;


private:
	CGLES3SamplerManager(void);
	virtual ~CGLES3SamplerManager(void);


private:
	CGLES3Sampler* Create(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);


private:
	eastl::unordered_map<uint32_t, CGLES3Sampler*> m_pSamplers;
};
