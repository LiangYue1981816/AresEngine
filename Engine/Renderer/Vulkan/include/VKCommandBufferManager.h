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
	CVKDevice *m_pDevice;
};
