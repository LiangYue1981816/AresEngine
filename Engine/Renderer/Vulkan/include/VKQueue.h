#pragma once
#include "VKRenderer.h"


class CVKQueue
{
	friend class CVKDevice;


private:
	CVKQueue(CVKDevice *pDevice, uint32_t queueFamilyIndex, uint32_t queueIndex);
	virtual ~CVKQueue(void);


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
	CVKDevice *m_pDevice;
};
