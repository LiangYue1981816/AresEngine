#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	CGfxFrameBuffer(GLuint width, GLuint height, bool bDepthRenderBuffer);
	virtual ~CGfxFrameBuffer(void);
	virtual void Release(void);


public:
	bool SetDepthTexture(CGfxTexture2DPtr &ptrTexture);
	bool SetColorTexture(GLuint index, CGfxTexture2DPtr &ptrTexture, bool invalidation);
	bool Apply(void);

public:
	GLuint GetWidth(void) const;
	GLuint GetHeight(void) const;

	CGfxTexture2DPtr GetDepthTexture(void) const;
	CGfxTexture2DPtr GetColorTexture(GLuint index) const;

private:
	void Bind(void) const;
	void InvalidateFramebuffer(void) const;


private:
	GLuint m_width;
	GLuint m_height;

private:
	GLuint m_fbo;
	GLuint m_rbo;

	CGfxTexture2DPtr m_ptrDepthTexture;
	eastl::unordered_map<GLuint, CGfxTexture2DPtr> m_ptrColorTextures;
	eastl::unordered_map<GLuint, bool> m_invalidations;
};
