#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
	friend class CVKRenderer;


private:
	CVKDescriptorSetManager(CVKDevice *pDevice);
	virtual ~CVKDescriptorSetManager(void);


private:
	CVKDescriptorSet* AllocDescriptorSet(CVKDescriptorLayout *pDescriptorLayout);
	void FreeDescriptorSet(CVKDescriptorSet *pDescriptorSet);


private:
	pthread_mutex_t m_lock;
	CVKDescriptorPool *m_pListHead;

private:
	CVKDevice *m_pDevice;
};
