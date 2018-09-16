#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	CGfxFrameBuffer(GLuint width, GLuint height);
	virtual ~CGfxFrameBuffer(void);


public:
	void Retain(void);
	void Release(void);

public:
	bool SetRenderTexture(GLuint index, CGfxTexture2D *pTexture, bool invalidation);
	bool CheckFramebufferStatus(void);

public:
	GLuint GetWidth(void) const;
	GLuint GetHeight(void) const;

private:
	void Bind(void);
	void InvalidateFramebuffer(void);


private:
	GLuint m_width;
	GLuint m_height;

private:
	GLuint m_fbo;
	GLuint m_rbo;
	eastl::unordered_map<GLuint, CGfxTexture2D*> m_pTextures;
	eastl::unordered_map<GLuint, bool> m_invalidations;

private:
	GLuint refCount;
};
