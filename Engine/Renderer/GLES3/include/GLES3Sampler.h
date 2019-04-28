#pragma once
#include "GLES3Renderer.h"


class CGLES3Sampler : public CGfxSampler
{
	friend class CGLES3SamplerManager;


private:
	CGLES3Sampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);
	virtual ~CGLES3Sampler(void);


public:
	void Bind(uint32_t unit) const;


private:
	uint32_t m_sampler;
};
