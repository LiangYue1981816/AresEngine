#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
	friend class CVKDevice;
	friend class CVKDescriptorSet;
	friend class CVKDescriptorPool;


private:
	CVKDescriptorSetManager(CVKDevice* pDevice);
	virtual ~CVKDescriptorSetManager(void);


public:
	CVKDescriptorSet* AllocDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	void FreeDescriptorSet(CVKDescriptorSet* pDescriptorSet);


private:
	pthread_mutex_t lock;
	CVKDescriptorPool* m_pPoolListHead;

private:
	CVKDevice* m_pDevice;
};
