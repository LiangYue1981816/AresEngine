#pragma once
#include "GfxRenderer.h"


class CGfxSamplerManager
{
	friend class CGfxRenderer;


private:
	CGfxSamplerManager(void);
	virtual ~CGfxSamplerManager(void);


private:
	CGfxSampler* CreateSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode);


private:
	eastl::unordered_map<GLuint64, CGfxSampler*> m_pSamplers;
};
