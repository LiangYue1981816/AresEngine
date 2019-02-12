#include "GLES3Renderer.h"


CGLES3Sampler::CGLES3Sampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
	: CGfxSampler(minFilter, magFilter, mipmapMode, addressMode)

	, m_sampler(0)
{
	glGenSamplers(1, &m_sampler);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, CGLES3Helper::TranslateMinFilter(minFilter, mipmapMode));
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, CGLES3Helper::TranslateMagFilter(magFilter));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, CGLES3Helper::TranslateAddressMode(addressMode));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, CGLES3Helper::TranslateAddressMode(addressMode));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, CGLES3Helper::TranslateAddressMode(addressMode));
}

CGLES3Sampler::~CGLES3Sampler(void)
{
	glDeleteSamplers(1, &m_sampler);
}

HANDLE CGLES3Sampler::GetSampler(void) const
{
	return (HANDLE)m_sampler;
}

void CGLES3Sampler::Bind(uint32_t unit)
{
	GLBindSampler(unit, m_sampler);
}
