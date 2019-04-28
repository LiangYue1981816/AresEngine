#pragma once
#include "VKRenderer.h"


class CVKCommandPool
{
	friend class CVKCommandBuffer;
	friend class CVKCommandBufferManager;


private:
	CVKCommandPool(CVKDevice* pDevice);
	virtual ~CVKCommandPool(void);


private:
	VkCommandPool GetCommandPool(void) const;

private:
	CVKCommandBuffer* AllocCommandBuffer(bool bMainCommandBuffer);
	void FreeCommandBuffer(CVKCommandBuffer* pCommandBuffer);
	void ResetCommandPool(void);


private:
	VkCommandPool m_vkCommandPool;
	eastl::unordered_map<CVKCommandBuffer*, CVKCommandBuffer*> m_pCommandBuffers;

private:
	CVKDevice* m_pDevice;
};
