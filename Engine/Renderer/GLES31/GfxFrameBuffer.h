#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	CGfxFrameBuffer(GLuint width, GLuint height, bool bDepthRenderBuffer);
	virtual ~CGfxFrameBuffer(void);


public:
	void Retain(void);
	void Release(void);

public:
	bool SetDepthTexture(CGfxTexture2D *pTexture);
	bool SetColorTexture(GLuint index, CGfxTexture2D *pTexture, bool invalidation);
	bool Apply(void);

public:
	GLuint GetWidth(void) const;
	GLuint GetHeight(void) const;

	CGfxTexture2D* GetDepthTexture(void) const;
	CGfxTexture2D* GetColorTexture(GLuint index) const;

private:
	void Bind(void);
	void InvalidateFramebuffer(void);


private:
	GLuint m_width;
	GLuint m_height;

private:
	GLuint m_fbo;
	GLuint m_rbo;

	CGfxTexture2D *m_pDepthTexture;
	eastl::unordered_map<GLuint, CGfxTexture2D*> m_pColorTextures;
	eastl::unordered_map<GLuint, bool> m_invalidations;

private:
	GLuint refCount;
};
