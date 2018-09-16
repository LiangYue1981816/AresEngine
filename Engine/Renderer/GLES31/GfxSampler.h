#pragma once
#include "GfxRenderer.h"


class CGfxSampler
{
	friend class CGfxMaterial;
	friend class CGfxSamplerManager;


private:
	CGfxSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode);
	virtual ~CGfxSampler(void);


private:
	GLuint GetSampler(void) const;


private:
	GLuint m_sampler;
};
