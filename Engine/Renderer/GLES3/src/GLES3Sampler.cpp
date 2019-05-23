#include "GLES3Renderer.h"


CGLES3Sampler::CGLES3Sampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
	: CGfxSampler(minFilter, magFilter, mipmapMode, addressMode)
	, m_sampler(0)
{
	Create(minFilter, magFilter, mipmapMode, addressMode);
}

CGLES3Sampler::~CGLES3Sampler(void)
{
	Destroy();
}

bool CGLES3Sampler::Create(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	glGenSamplers(1, &m_sampler);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, CGLES3Helper::TranslateMinFilter(minFilter, mipmapMode));
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, CGLES3Helper::TranslateMagFilter(magFilter));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, CGLES3Helper::TranslateAddressMode(addressMode));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, CGLES3Helper::TranslateAddressMode(addressMode));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, CGLES3Helper::TranslateAddressMode(addressMode));
	CHECK_GL_ERROR_ASSERT();

	return true;
}

void CGLES3Sampler::Destroy(void)
{
	ASSERT(m_sampler);

	glDeleteSamplers(1, &m_sampler);
	m_sampler = 0;
}

void CGLES3Sampler::Bind(uint32_t unit) const
{
	ASSERT(m_sampler);

	GLBindSampler(unit, m_sampler);
	CHECK_GL_ERROR_ASSERT();
}
