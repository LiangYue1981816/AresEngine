#pragma once
#include "VKRenderer.h"


class CVKCommandPool
{
	friend class CVKCommandBufferManager;


private:
	CVKCommandPool(CVKDevice *pDevice, uint32_t queueFamilyIndex);
	virtual ~CVKCommandPool(void);


private:
	CVKCommandBuffer* AllocCommandBuffer(bool bMainCommandBuffer);
	void FreeCommandBuffer(CVKCommandBuffer *pCommandBuffer);
	void ResetCommandPool(void);


private:
	eastl::unordered_map<CVKCommandBuffer*, CVKCommandBuffer*> m_pCommandBuffers;

private:
	VkCommandPool m_vkCommandPool;

private:
	CVKDevice *m_pDevice;
};
