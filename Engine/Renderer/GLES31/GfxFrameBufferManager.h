#pragma once
#include "GfxRenderer.h"


class CGfxFrameBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxFrameBufferManager(void);
	virtual ~CGfxFrameBufferManager(void);


private:
	CGfxFrameBuffer* CreateFrameBuffer(GLuint width, GLuint height, bool bDepthRenderBuffer);
	void DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer);


private:
	eastl::unordered_map<CGfxFrameBuffer*, CGfxFrameBuffer*> m_pFrameBuffers;
};
