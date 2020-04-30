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

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Get(uint32_t name)
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

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Create(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pDescriptorSets[name] == nullptr) {
			m_pDescriptorSets[name] = new CGLES3DescriptorSet(this, name, ptrDescriptorLayout);
		}

		return m_pDescriptorSets[name];
	}
}

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Create(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pDescriptorSets[name] == nullptr) {
			m_pDescriptorSets[name] = new CGLES3DescriptorSet(this, name, ptrDescriptorSetCopyFrom);
		}

		return m_pDescriptorSets[name];
	}
}

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	mutex_autolock autolock(&lock);
	{
		if (const SubpassInformation* pSubpassInformation = pRenderPass->GetSubpass(indexSubpass)) {
			if (pSubpassInformation->inputAttachments.size()) {
				if (m_pInputAttachmentDescriptorSets[(CGLES3FrameBuffer*)pFrameBuffer][(SubpassInformation*)pSubpassInformation][(CGLES3PipelineGraphics*)pPipelineGraphics] == nullptr) {
					CGLES3DescriptorSet* pDescriptorSet = new CGLES3DescriptorSet(this, INVALID_HASHNAME, pPipelineGraphics->GetDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT));
					{
						for (const auto& itInputAttachment : pSubpassInformation->inputAttachments) {
							pDescriptorSet->SetInputAttachmentTexture(
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
	ASSERT(pDescriptorSet);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pDescriptorSets.find(pDescriptorSet->GetName()) != m_pDescriptorSets.end()) {
				m_pDescriptorSets.erase(pDescriptorSet->GetName());
				delete pDescriptorSet;
			}
		}
	}
}
