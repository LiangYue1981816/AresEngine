#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
	friend class CVKDevice;


private:
	CVKDescriptorSetManager(CVKDevice *pDevice);
	virtual ~CVKDescriptorSetManager(void);


public:
	CVKDescriptorSet* AllocDescriptorSet(uint32_t pool, CVKDescriptorLayout *pDescriptorLayout);
	void FreeDescriptorSet(CVKDescriptorSet *pDescriptorSet);


private:
	pthread_mutex_t m_lock;
	eastl::unordered_map<uint32_t, CVKDescriptorPool*> m_pPoolListHeads;

private:
	CVKDevice *m_pDevice;
};
