#pragma once
#include "GLES3Renderer.h"


class CGLES3SamplerManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Sampler;


private:
	CGLES3SamplerManager(void);
	virtual ~CGLES3SamplerManager(void);


private:
	CGLES3Sampler* Create(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);


private:
	eastl::unordered_map<uint32_t, CGLES3Sampler*> m_pSamplers;
};
