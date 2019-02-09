#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{
	pthread_mutex_init(&m_lock, nullptr);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	for (const auto &itPool : m_pPoolListHeads) {
		if (CVKDescriptorPool *pDescriptorPool = itPool.second) {
			CVKDescriptorPool *pDescriptorPoolNext = nullptr;
			do {
				pDescriptorPoolNext = pDescriptorPool->pNext;
				delete pDescriptorPool;
			} while (pDescriptorPool = pDescriptorPoolNext);
		}
	}

	m_pPoolListHeads.clear();
	pthread_mutex_destroy(&m_lock);
}

CVKDescriptorSet* CVKDescriptorSetManager::AllocDescriptorSet(uint32_t pool, CVKDescriptorLayout *pDescriptorLayout)
{
	mutex_autolock autolock(&m_lock);
	{
		/*
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
		*/
	}

	return nullptr;
}

void CVKDescriptorSetManager::FreeDescriptorSet(CVKDescriptorSet *pDescriptorSet)
{
	/*
	if (pDescriptorSet == nullptr) {
		return;
	}

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
	*/
}
