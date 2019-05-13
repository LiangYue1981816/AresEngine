#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_pPoolListHead(nullptr)
{
	ASSERT(m_pDevice);

	pthread_mutex_init(&lock, nullptr);
	m_pPoolListHead = new CVKDescriptorPool(m_pDevice);
	m_pInputAttachmentPoolListHead = new CVKDescriptorPool(m_pDevice);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	if (CVKDescriptorPool* pDescriptorPool = m_pPoolListHead) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while (pDescriptorPool = pDescriptorPoolNext);
	}

	if (CVKDescriptorPool* pDescriptorPool = m_pInputAttachmentPoolListHead) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while (pDescriptorPool = pDescriptorPoolNext);
	}

	pthread_mutex_destroy(&lock);
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
