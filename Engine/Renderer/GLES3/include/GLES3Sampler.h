#pragma once
#include "GLES3Renderer.h"


class CGLES3Sampler : public CGfxSampler
{
	friend class CGLES3SamplerManager;


private:
	CGLES3Sampler(GfxFilter minFilter, GfxFilter magFilter, GfxMipmapMode mipmapMode, GfxAddressMode addressMode);
	virtual ~CGLES3Sampler(void);


public:
	void Bind(uint32_t unit);


private:
	uint32_t m_sampler;
};
