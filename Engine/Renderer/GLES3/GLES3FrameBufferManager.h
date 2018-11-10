#pragma once
#include "GLES3Renderer.h"


class CGLES3FrameBufferManager
{
	friend class CGLES3Renderer;
	friend class CGLES3FrameBuffer;


private:
	CGLES3FrameBufferManager(void);
	virtual ~CGLES3FrameBufferManager(void);


private:
	CGLES3FrameBuffer* CreateFrameBuffer(int width, int height);
	void DestroyFrameBuffer(CGLES3FrameBuffer *pFrameBuffer);


private:
	eastl::unordered_map<CGLES3FrameBuffer*, CGLES3FrameBuffer*> m_pFrameBuffers;
};
