#pragma once
#include "GfxRenderer.h"


class CGfxFrameBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxFrameBufferManager(void);
	virtual ~CGfxFrameBufferManager(void);


private:
	CGfxFrameBuffer* CreateFrameBuffer(GLuint width, GLuint height);
	void DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer);


private:
	eastl::unordered_map<CGfxFrameBuffer*, CGfxFrameBuffer*> m_pFrameBuffers;
};
