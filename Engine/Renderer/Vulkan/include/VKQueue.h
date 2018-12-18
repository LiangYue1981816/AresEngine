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


private:
	VkQueue m_vkQueue;
};
