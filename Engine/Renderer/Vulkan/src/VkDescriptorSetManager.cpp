#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_pPoolListHead(nullptr)
	, m_pInputAttachmentPoolListHead(nullptr)
{
	ASSERT(m_pDevice);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	if (CVKDescriptorPool* pDescriptorPool = m_pPoolListHead) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while ((pDescriptorPool = pDescriptorPoolNext) != nullptr);
	}

	if (CVKDescriptorPool* pDescriptorPool = m_pInputAttachmentPoolListHead) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while ((pDescriptorPool = pDescriptorPoolNext) != nullptr);
	}
}

CVKDescriptorSet* CVKDescriptorSetManager::CreateInternal(CVKDescriptorPool** ppPoolListHead, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	do {
		if (CVKDescriptorPool* pDescriptorPool = *ppPoolListHead) {
			do {
				if (CVKDescriptorSet* pDescriptorSet = pDescriptorPool->AllocDescriptorSet(ptrDescriptorLayout)) {
					return pDescriptorSet;
				}
			} while ((pDescriptorPool = pDescriptorPool->pNext) != nullptr);
		}

		CVKDescriptorPool* pDescriptorPool = new CVKDescriptorPool(m_pDevice);
		{
			if ((*ppPoolListHead) != nullptr) {
				(*ppPoolListHead)->pPrev = pDescriptorPool;
				pDescriptorPool->pNext = (*ppPoolListHead);
			}

			*ppPoolListHead = pDescriptorPool;
		}
	} while (true);
}

CVKDescriptorSet* CVKDescriptorSetManager::Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		CVKDescriptorSet* pDescriptorSet = CreateInternal(&m_pPoolListHead, ptrDescriptorLayout);
		m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;
		return pDescriptorSet;
	}
}

CVKDescriptorSet* CVKDescriptorSetManager::Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	mutex_autolock autolock(&lock);
	{
		if (const SubpassInformation* pSubpassInformation = pRenderPass->GetSubpass(indexSubpass)) {
			if (pSubpassInformation->inputAttachments.size()) {
				if (m_pInputAttachmentDescriptorSets[(CVKFrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CVKPipelineGraphics*)pPipelineGraphics] == nullptr) {
					CVKDescriptorSet* pDescriptorSet = CreateInternal(&m_pInputAttachmentPoolListHead, pPipelineGraphics->GetDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT));
					{
						for (const auto& itInputAttachment : pSubpassInformation->inputAttachments) {
							pDescriptorSet->SetTextureInputAttachment(
								((CVKPipelineGraphics*)pPipelineGraphics)->GetInputAttachmentName(itInputAttachment.first),
								((CVKFrameBuffer*)pFrameBuffer)->GetAttachmentTexture(itInputAttachment.first),
								VKRenderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE));
						}
					}
					m_pInputAttachmentDescriptorSets[(CVKFrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CVKPipelineGraphics*)pPipelineGraphics] = pDescriptorSet;
				}

				return m_pInputAttachmentDescriptorSets[(CVKFrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CVKPipelineGraphics*)pPipelineGraphics];
			}
		}

		return nullptr;
	}
}

void CVKDescriptorSetManager::Destroy(CVKDescriptorSet* pDescriptorSet)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pDescriptorSet);
		ASSERT(pDescriptorSet->GetDescriptorPool());

		if (m_pDescriptorSets.find(pDescriptorSet) != m_pDescriptorSets.end()) {
			m_pDescriptorSets.erase(pDescriptorSet);

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
}
