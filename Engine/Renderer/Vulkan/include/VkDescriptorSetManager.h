#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDescriptorSet;
	friend class CVKDescriptorPool;


private:
	CVKDescriptorSetManager(CVKDevice* pDevice);
	virtual ~CVKDescriptorSetManager(void);


private:
	CVKDescriptorSet* Get(uint32_t name);
	CVKDescriptorSet* CreateInternal(CVKDescriptorPool** ppPoolListHead, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CVKDescriptorSet* Create(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CVKDescriptorSet* Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass);
	void Destroy(CVKDescriptorSet* pDescriptorSet);


private:
	CVKDescriptorPool* m_pPoolListHead;
	CVKDescriptorPool* m_pInputAttachmentPoolListHead;
	eastl::unordered_map<uint32_t, CVKDescriptorSet*> m_pDescriptorSets;
	eastl::unordered_map<CVKFrameBuffer*, eastl::unordered_map<SubpassInformation*, eastl::unordered_map<CVKPipelineGraphics*, CVKDescriptorSet*>>> m_pInputAttachmentDescriptorSets;

private:
	CVKDevice* m_pDevice;
};
