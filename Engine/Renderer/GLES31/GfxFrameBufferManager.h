#pragma once
#include "GfxRenderer.h"


class CGfxFrameBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxFrameBufferManager(void);
	virtual ~CGfxFrameBufferManager(void);


private:
	CGfxFrameBuffer* CreateFrameBuffer(uint32_t width, uint32_t height);
	CGfxFrameBuffer* CreateFrameBuffer(uint32_t width, uint32_t height, bool bDepthRenderBuffer, int samples = 0);
	void DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer);


private:
	eastl::unordered_map<CGfxFrameBuffer*, CGfxFrameBuffer*> m_pFrameBuffers;
};
