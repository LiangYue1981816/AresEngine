#pragma once
#include "VKRenderer.h"


class CVKQueue
{
	friend class CVKDevice;


private:
	CVKQueue(CVKDevice *pDevice, uint32_t queueFamilyIndex, uint32_t queueIndex);
	virtual ~CVKQueue(void);


public:
	bool ResetCommandBufferPool(uint32_t pool);
	bool AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level, VkCommandBuffer *pvkCommandBuffer);
	void FreeCommandBuffer(uint32_t pool, VkCommandBuffer vkCommandBuffer);

public:
	bool Submit(const CGfxCommandBufferPtr &ptrCommandBuffer) const;
	bool Submit(const CGfxCommandBufferPtr &ptrCommandBuffer, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore) const;
	void WaitIdle(void) const;

public:
	VkQueue GetQueue(void) const;
	uint32_t GetQueueFamilyIndex(void) const;


private:
	VkQueue m_vkQueue;
	uint32_t m_queueFamilyIndex;

private:
	std::atomic_flag m_lock;
	eastl::unordered_map<uint32_t, VkCommandPool> m_vkCommandPools;

private:
	CVKDevice *m_pDevice;
};
