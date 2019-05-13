#include "GLES3Renderer.h"


CGLES3DescriptorSetManager::CGLES3DescriptorSetManager(void)
{

}

CGLES3DescriptorSetManager::~CGLES3DescriptorSetManager(void)
{
	for (const auto& itDescriptorSet : m_pDescriptorSets) {
		delete itDescriptorSet.second;
	}

	for (const auto& itFrameBufferDescriptorSet : m_pInputAttachmentDescriptorSets) {
		for (const auto& itSubpassDescriptorSet : itFrameBufferDescriptorSet.second) {
			for (const auto& itPipelineDescriptorSet : itSubpassDescriptorSet.second) {
				delete itPipelineDescriptorSet.second;
			}
		}
	}
}

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3DescriptorSet* pDescriptorSet = new CGLES3DescriptorSet(this, ptrDescriptorLayout);
		m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;
		return pDescriptorSet;
	}
}

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	mutex_autolock autolock(&lock);
	{
		if (const SubpassInformation* pSubpassInformation = pRenderPass->GetSubpass(indexSubpass)) {
			if (pSubpassInformation->inputAttachments.size()) {
				if (m_pInputAttachmentDescriptorSets[(CGLES3FrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CGLES3PipelineGraphics*)pPipelineGraphics] == nullptr) {
					CGLES3DescriptorSet* pDescriptorSet = new CGLES3DescriptorSet(this, pPipelineGraphics->GetDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT));
					{
						for (const auto& itInputAttachment : pSubpassInformation->inputAttachments) {
							pDescriptorSet->SetTextureInputAttachment(
								((CGLES3PipelineGraphics*)pPipelineGraphics)->GetInputAttachmentName(itInputAttachment.first),
								((CGLES3FrameBuffer*)pFrameBuffer)->GetAttachmentTexture(itInputAttachment.first),
								GLES3Renderer()->CreateSampler(GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE));
						}
					}
					m_pInputAttachmentDescriptorSets[(CGLES3FrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CGLES3PipelineGraphics*)pPipelineGraphics] = pDescriptorSet;
				}

				return m_pInputAttachmentDescriptorSets[(CGLES3FrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CGLES3PipelineGraphics*)pPipelineGraphics];
			}
		}

		return nullptr;
	}
}

void CGLES3DescriptorSetManager::Destroy(CGLES3DescriptorSet* pDescriptorSet)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pDescriptorSet);

		if (m_pDescriptorSets.find(pDescriptorSet) != m_pDescriptorSets.end()) {
			m_pDescriptorSets.erase(pDescriptorSet);
			delete pDescriptorSet;
		}
	}
}
