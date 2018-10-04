#pragma once
#include "GfxRenderer.h"


class CGfxSampler
{
	friend class CGfxRenderer;
	friend class CGfxSamplerManager;


private:
	CGfxSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode);
	virtual ~CGfxSampler(void);


public:
	GLuint GetSampler(void) const;


private:
	GLuint m_sampler;
};
