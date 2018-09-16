#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxSampler.h"


CGfxSampler::CGfxSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode)
	: m_sampler(0)
{
	glGenSamplers(1, &m_sampler);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, addressMode);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, addressMode);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, addressMode);
}

CGfxSampler::~CGfxSampler(void)
{
	glDeleteSamplers(1, &m_sampler);
}

GLuint CGfxSampler::GetSampler(void) const
{
	return m_sampler;
}
