#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
	, m_pDescriptorPoolListHead(nullptr)
{
	pthread_mutex_init(&m_lock, nullptr);
	m_pDescriptorPoolListHead = new CVKDescriptorPool(m_pDevice);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	if (CVKDescriptorPool *pDescriptorPool = m_pDescriptorPoolListHead) {
		CVKDescriptorPool *pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while (pDescriptorPool = pDescriptorPoolNext);
	}

	m_pDescriptorPoolListHead = nullptr;
	pthread_mutex_destroy(&m_lock);
}

CVKDescriptorSet* CVKDescriptorSetManager::AllocDescriptorSet(CVKDescriptorLayout *pDescriptorLayout)
{
	return nullptr;
}

void CVKDescriptorSetManager::FreeDescriptorSet(CVKDescriptorSet *pDescriptorSet)
{

}
