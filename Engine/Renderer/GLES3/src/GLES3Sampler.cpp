#include "GfxHeader.h"


CGLES3Sampler::CGLES3Sampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
	: CGfxSampler(minFilter, magFilter, addressMode)
	, m_sampler(0)
{
	glGenSamplers(1, &m_sampler);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, addressMode);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, addressMode);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, addressMode);
}

CGLES3Sampler::~CGLES3Sampler(void)
{
	glDeleteSamplers(1, &m_sampler);
}

void CGLES3Sampler::Bind(uint32_t unit)
{
	GLBindSampler(unit, m_sampler);
}
