#pragma once
#include "VKRenderer.h"


class CVKCommandBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKCommandBuffer;


private:
	CVKCommandBufferManager(CVKDevice* pDevice);
	virtual ~CVKCommandBufferManager(void);


private:
	CVKCommandBuffer* Create(uint32_t pool, bool bMainCommandBuffer);
	void Destroy(CVKCommandBuffer* pCommandBuffer);


private:
	eastl::unordered_map<uint32_t, VkCommandPool> m_vkCommandPools;
	eastl::unordered_map<VkCommandPool, eastl::unordered_map<bool, eastl::list<CVKCommandBuffer*>>> m_pCommandBuffers;

private:
	CVKDevice* m_pDevice;
};
