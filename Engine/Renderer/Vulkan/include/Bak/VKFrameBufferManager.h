#pragma once
#include "VKRenderer.h"


class CVKFrameBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKFrameBuffer;


private:
	CVKFrameBufferManager(CVKDevice *pDevice);
	virtual ~CVKFrameBufferManager(void);


private:
	CVKFrameBuffer* Create(int width, int height, int numAttachments);
	void Destroy(CVKFrameBuffer *pFrameBuffer);


private:
	eastl::unordered_map<CVKFrameBuffer*, CVKFrameBuffer*> m_pFrameBuffers;

private:
	CVKDevice *m_pDevice;
};
