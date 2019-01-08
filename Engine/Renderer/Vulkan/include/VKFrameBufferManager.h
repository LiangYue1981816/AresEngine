#pragma once
#include "VKRenderer.h"


class CVKFrameBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDevice;
	friend class CVKFrameBuffer;


private:
	CVKFrameBufferManager(CVKDevice *pDevice);
	virtual ~CVKFrameBufferManager(void);


private:
	CVKFrameBuffer* Create(int width, int height);
	void Destroy(CGfxFrameBuffer *pFrameBuffer);


private:
	eastl::unordered_map<CGfxFrameBuffer*, CGfxFrameBuffer*> m_pFrameBuffers;

private:
	CVKDevice *m_pDevice;
};
