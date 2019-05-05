#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
	friend class CVKDevice;


private:
	CVKDescriptorSetManager(CVKDevice* pDevice);
	virtual ~CVKDescriptorSetManager(void);


public:
	CVKDescriptorSet* AllocDescriptorSet(CVKDescriptorLayout* pDescriptorLayout);
	void FreeDescriptorSet(CVKDescriptorSet* pDescriptorSet);


private:
	pthread_mutex_t lock;
	CVKDescriptorPool* m_pPoolListHead;

private:
	CVKDevice* m_pDevice;
};
