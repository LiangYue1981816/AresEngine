#pragma once
#include "VKRenderer.h"


class CVKCommandBufferPool
{
	friend class CVKCommandBufferManager;


private:
	CVKCommandBufferPool(CVKDevice *pDevice);
	virtual ~CVKCommandBufferPool(void);


private:
	VkCommandPool m_vkCommandPool;

private:
	CVKDevice *m_pDevice;
};
