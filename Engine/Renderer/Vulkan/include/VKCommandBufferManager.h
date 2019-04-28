#pragma once
#include "VKRenderer.h"


class CVKCommandBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKCommandPool;
	friend class CVKCommandBuffer;


private:
	CVKCommandBufferManager(CVKDevice* pDevice);
	virtual ~CVKCommandBufferManager(void);


private:
	CVKCommandBuffer* Create(uint32_t pool, bool bMainCommandBuffer);


private:
	pthread_mutex_t m_lock;
	eastl::unordered_map<uint32_t, CVKCommandPool*> m_pCommandPools;

private:
	CVKDevice* m_pDevice;
};
