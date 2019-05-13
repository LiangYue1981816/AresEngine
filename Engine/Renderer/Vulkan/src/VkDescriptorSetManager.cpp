#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_pPoolListHead(nullptr)
	, m_pInputAttachmentPoolListHead(nullptr)
{
	ASSERT(m_pDevice);

	pthread_mutex_init(&lock, nullptr);
	InitPool(&m_pPoolListHead);
	InitPool(&m_pInputAttachmentPoolListHead);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	FreePool(&m_pPoolListHead);
	FreePool(&m_pInputAttachmentPoolListHead);
	pthread_mutex_destroy(&lock);
}

void CVKDescriptorSetManager::InitPool(CVKDescriptorPool** ppPoolListHead)
{
	*ppPoolListHead = new CVKDescriptorPool(m_pDevice);
}

void CVKDescriptorSetManager::FreePool(CVKDescriptorPool** ppPoolListHead)
{
	if (CVKDescriptorPool* pDescriptorPool = *ppPoolListHead) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while (pDescriptorPool = pDescriptorPoolNext);
	}

	*ppPoolListHead = nullptr;
}

CVKDescriptorSet* CVKDescriptorSetManager::Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		CVKDescriptorPool* pDescriptorPool = m_pPoolListHead;

		do {
			if (CVKDescriptorSet* DescriptorSet = pDescriptorPool->AllocDescriptorSet(ptrDescriptorLayout)) {
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

CVKDescriptorSet* CVKDescriptorSetManager::Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	return nullptr;
}

void CVKDescriptorSetManager::Destroy(CVKDescriptorSet* pDescriptorSet)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pDescriptorSet);
		ASSERT(pDescriptorSet->GetDescriptorPool());

		CVKDescriptorPool* pDescriptorPool = pDescriptorSet->GetDescriptorPool();

		if (pDescriptorPool->FreeDescriptorSet(pDescriptorSet)) {
			if (pDescriptorPool->pPrev) {
				pDescriptorPool->pPrev->pNext = pDescriptorPool->pNext;
			}

			if (pDescriptorPool->pNext) {
				pDescriptorPool->pNext->pPrev = pDescriptorPool->pPrev;
			}

			pDescriptorPool->pPrev = nullptr;
			pDescriptorPool->pNext = m_pPoolListHead;
			m_pPoolListHead = pDescriptorPool;
		}
	}
}
