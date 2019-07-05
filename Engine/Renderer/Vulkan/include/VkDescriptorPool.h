#pragma once
#include "VKRenderer.h"


class CVKDescriptorPool
{
	friend class CVKDescriptorSet;
	friend class CVKDescriptorSetManager;


private:
	CVKDescriptorPool(CVKDevice* pDevice, CVKDescriptorSetManager* pManager);
	virtual ~CVKDescriptorPool(void);


private:
	bool Create(void);
	void Destroy(void);

private:
	VkDescriptorPool GetDescriptorPool(void) const;
	CVKDescriptorSetManager* GetDescriptorSetManager(void) const;

private:
	CVKDescriptorSet* AllocDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CVKDescriptorSet* AllocDescriptorSet(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom);
	bool FreeDescriptorSet(CVKDescriptorSet* pDescriptorSet);


private:
	VkDescriptorPool m_vkDescriptorPool;

private:
	uint32_t m_numSets;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<CVKDescriptorSet*, CVKDescriptorSet*> m_pDescriptorSets;

private:
	CVKDevice* m_pDevice;

private:
	CVKDescriptorSetManager* m_pManager;

private:
	CVKDescriptorPool* pNext;
	CVKDescriptorPool* pPrev;
};
