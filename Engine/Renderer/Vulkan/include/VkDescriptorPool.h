#pragma once
#include "VKRenderer.h"


class CVKDescriptorPool
{
private:
	CVKDescriptorPool(CVKDevice *pDevice);
	virtual ~CVKDescriptorPool(void);


private:
	CVKDescriptorSet* AllocDescriptorSet(CVKDescriptorLayout *pDescriptorLayout);
	void FreeDescriptorSet(CVKDescriptorSet *pDescriptorSet);


private:
	uint32_t m_numSets;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<CVKDescriptorSet*, CVKDescriptorSet*> m_pDescriptorSets;

private:
	VkDescriptorPool m_vkDescriptorPool;

private:
	CVKDevice *m_pDevice;
};
