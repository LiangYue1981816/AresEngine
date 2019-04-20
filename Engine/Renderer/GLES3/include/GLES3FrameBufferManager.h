#pragma once
#include "GLES3Renderer.h"


class CGLES3FrameBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3FrameBuffer;


private:
	CGLES3FrameBufferManager(void);
	virtual ~CGLES3FrameBufferManager(void);


private:
	CGLES3FrameBuffer* Create(int width, int height, int numAttachments);
	void Destroy(CGLES3FrameBuffer* pFrameBuffer);


private:
	eastl::unordered_map<CGLES3FrameBuffer*, CGLES3FrameBuffer*> m_pFrameBuffers;
};
