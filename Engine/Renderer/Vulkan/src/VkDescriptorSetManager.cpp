#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
	, m_pListHead(nullptr)
{
	pthread_mutex_init(&m_lock, nullptr);
	m_pListHead = new CVKDescriptorPool(m_pDevice);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	if (CVKDescriptorPool *pDescriptorPool = m_pListHead) {
		CVKDescriptorPool *pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while (pDescriptorPool = pDescriptorPoolNext);
	}

	m_pListHead = nullptr;
	pthread_mutex_destroy(&m_lock);
}

CVKDescriptorSet* CVKDescriptorSetManager::AllocDescriptorSet(CVKDescriptorLayout *pDescriptorLayout)
{
	mutex_autolock autolock(&m_lock);
	{
		CVKDescriptorPool *pDescriptorPool = m_pListHead;

		do {
			if (CVKDescriptorSet *DescriptorSet = pDescriptorPool->AllocDescriptorSet(pDescriptorLayout)) {
				return DescriptorSet;
			}

			if (pDescriptorPool->pNext == nullptr) {
				pDescriptorPool->pNext = new CVKDescriptorPool(m_pDevice);
			}

			pDescriptorPool = pDescriptorPool->pNext;
		} while (true);
	}

	return nullptr;
}

void CVKDescriptorSetManager::FreeDescriptorSet(CVKDescriptorSet *pDescriptorSet)
{
	mutex_autolock autolock(&m_lock);
	{
		CVKDescriptorPool *pDescriptorPool = pDescriptorSet->GetDescriptorPool();

		if (pDescriptorPool->FreeDescriptorSet(pDescriptorSet)) {
			if (pDescriptorPool->pPrev) {
				pDescriptorPool->pPrev->pNext = pDescriptorPool->pNext;
			}

			if (pDescriptorPool->pNext) {
				pDescriptorPool->pNext->pPrev = pDescriptorPool->pPrev;
			}

			pDescriptorPool->pPrev = nullptr;
			pDescriptorPool->pNext = m_pListHead;
			m_pListHead = pDescriptorPool;
		}
	}
}
