#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)

	, m_pGeneralPoolList(nullptr)
	, m_pInputAttachmentPoolList(nullptr)
{

}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	if (CVKDescriptorPool* pDescriptorPool = m_pGeneralPoolList) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while ((pDescriptorPool = pDescriptorPoolNext) != nullptr);
	}

	if (CVKDescriptorPool* pDescriptorPool = m_pInputAttachmentPoolList) {
		CVKDescriptorPool* pDescriptorPoolNext = nullptr;
		do {
			pDescriptorPoolNext = pDescriptorPool->pNext;
			delete pDescriptorPool;
		} while ((pDescriptorPool = pDescriptorPoolNext) != nullptr);
	}
}

CVKDescriptorSet* CVKDescriptorSetManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itDescriptorSet = m_pDescriptorSets.find(name);

		if (itDescriptorSet != m_pDescriptorSets.end()) {
			return itDescriptorSet->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKDescriptorSet* CVKDescriptorSetManager::CreateInternal(CVKDescriptorPool** ppPoolList, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	do {
		if (CVKDescriptorPool* pDescriptorPool = *ppPoolList) {
			do {
				if (CVKDescriptorSet* pDescriptorSet = pDescriptorPool->AllocDescriptorSet(name, ptrDescriptorLayout)) {
					return pDescriptorSet;
				}
			} while ((pDescriptorPool = pDescriptorPool->pNext) != nullptr);
		}

		CVKDescriptorPool* pDescriptorPool = new CVKDescriptorPool(m_pDevice, this);
		{
			if ((*ppPoolList) != nullptr) {
				(*ppPoolList)->pPrev = pDescriptorPool;
				pDescriptorPool->pNext = (*ppPoolList);
			}

			*ppPoolList = pDescriptorPool;
		}
	} while (true);
}

CVKDescriptorSet* CVKDescriptorSetManager::CreateInternal(CVKDescriptorPool** ppPoolList, uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
{
	do {
		if (CVKDescriptorPool* pDescriptorPool = *ppPoolList) {
			do {
				if (CVKDescriptorSet* pDescriptorSet = pDescriptorPool->AllocDescriptorSet(name, ptrDescriptorSetCopyFrom)) {
					return pDescriptorSet;
				}
			} while ((pDescriptorPool = pDescriptorPool->pNext) != nullptr);
		}

		CVKDescriptorPool* pDescriptorPool = new CVKDescriptorPool(m_pDevice, this);
		{
			if ((*ppPoolList) != nullptr) {
				(*ppPoolList)->pPrev = pDescriptorPool;
				pDescriptorPool->pNext = (*ppPoolList);
			}

			*ppPoolList = pDescriptorPool;
		}
	} while (true);
}

CVKDescriptorSet* CVKDescriptorSetManager::Create(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pDescriptorSets[name] == nullptr) {
			m_pDescriptorSets[name] = CreateInternal(&m_pGeneralPoolList, name, ptrDescriptorLayout);
		}

		return m_pDescriptorSets[name];
	}
}

CVKDescriptorSet* CVKDescriptorSetManager::Create(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pDescriptorSets[name] == nullptr) {
			m_pDescriptorSets[name] = CreateInternal(&m_pGeneralPoolList, name, ptrDescriptorSetCopyFrom);
		}

		return m_pDescriptorSets[name];
	}
}

CVKDescriptorSet* CVKDescriptorSetManager::Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	mutex_autolock autolock(&lock);
	{
		if (const SubpassInformation* pSubpassInformation = pRenderPass->GetSubpass(indexSubpass)) {
			if (pSubpassInformation->inputAttachments.empty() == false) {
				if (m_pInputAttachmentDescriptorSets[(CVKFrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CVKPipelineGraphics*)pPipelineGraphics] == nullptr) {
					CVKDescriptorSet* pDescriptorSet = CreateInternal(&m_pInputAttachmentPoolList, INVALID_HASHNAME, pPipelineGraphics->GetDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT));
					{
						for (const auto& itInputAttachment : pSubpassInformation->inputAttachments) {
							pDescriptorSet->SetInputAttachmentTexture(
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
		m_pDescriptorSets.erase(pDescriptorSet->GetName());

		if (CVKDescriptorPool* pDescriptorPool = pDescriptorSet->GetDescriptorPool()) {
			if (pDescriptorPool->FreeDescriptorSet(pDescriptorSet)) {
				if (pDescriptorPool->pPrev) {
					pDescriptorPool->pPrev->pNext = pDescriptorPool->pNext;
				}

				if (pDescriptorPool->pNext) {
					pDescriptorPool->pNext->pPrev = pDescriptorPool->pPrev;
				}

				if (pDescriptorPool) {
					if (m_pGeneralPoolList == pDescriptorPool) {
						m_pGeneralPoolList =  pDescriptorPool->pNext;
					}
				}

				if (pDescriptorPool) {
					if (m_pInputAttachmentPoolList == pDescriptorPool) {
						m_pInputAttachmentPoolList =  pDescriptorPool->pNext;
					}
				}

				delete pDescriptorPool;
			}
		}
	}
}
