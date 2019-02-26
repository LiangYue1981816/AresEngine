#pragma once
#include "VKRenderer.h"


class CVKCommandBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKCommandBuffer;


private:
	CVKCommandBufferManager(CVKDevice *pDevice);
	virtual ~CVKCommandBufferManager(void);


private:
	CVKCommandBuffer* Create(bool bMainCommandBuffer);
	void Destroy(CVKCommandBuffer *pCommandBuffer);


private:
	eastl::unordered_map<CVKCommandBuffer*, CVKCommandBuffer*> m_pCommandBuffers;

private:
	CVKDevice *m_pDevice;
};
