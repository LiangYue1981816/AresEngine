#pragma once
#include "VKRenderer.h"


class CVKDescriptorPool
{
	friend class CVKDescriptorSet;
	friend class CVKDescriptorSetManager;


private:
	CVKDescriptorPool(CVKDevice* pDevice);
	virtual ~CVKDescriptorPool(void);


private:
	VkDescriptorPool GetDescriptorPool(void) const;

private:
	CVKDescriptorSet* AllocDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	bool FreeDescriptorSet(CVKDescriptorSet* pDescriptorSet);


private:
	uint32_t m_numSets;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<CVKDescriptorSet*, CVKDescriptorSet*> m_pDescriptorSets;

private:
	VkDescriptorPool m_vkDescriptorPool;

private:
	CVKDevice* m_pDevice;

private:
	CVKDescriptorPool* pNext;
	CVKDescriptorPool* pPrev;
};
