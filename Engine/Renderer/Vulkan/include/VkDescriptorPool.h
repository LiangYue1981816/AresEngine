#pragma once
#include "VKRenderer.h"


class CVKDescriptorPool
{
private:
	CVKDescriptorPool(CVKDevice *pDevice);
	virtual ~CVKDescriptorPool(void);


private:
	uint32_t m_numSets;
	uint32_t m_numFreeSets;
	uint32_t m_numFreeDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

private:
	VkDescriptorPool m_vkDescriptorPool;

private:
	CVKDevice *m_pDevice;
};
