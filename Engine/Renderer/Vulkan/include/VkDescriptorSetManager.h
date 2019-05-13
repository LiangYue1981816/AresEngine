#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
	friend class CVKDevice;
	friend class CVKDescriptorSet;
	friend class CVKDescriptorPool;


private:
	CVKDescriptorSetManager(CVKDevice* pDevice);
	virtual ~CVKDescriptorSetManager(void);


private:
	CVKDescriptorSet* CreateInternal(CVKDescriptorPool** ppPoolListHead, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	void DestroyPoolListInternal(CVKDescriptorPool** ppPoolListHead);

private:
	CVKDescriptorSet* Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CVKDescriptorSet* Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass);
	void Destroy(CVKDescriptorSet* pDescriptorSet);


private:
	pthread_mutex_t lock;
	CVKDescriptorPool* m_pPoolListHead;
	CVKDescriptorPool* m_pInputAttachmentPoolListHead;
	eastl::unordered_map<CVKDescriptorSet*, CVKDescriptorSet*> m_pDescriptorSets;
	eastl::unordered_map<CVKFrameBuffer*, eastl::unordered_map<SubpassInformation*, eastl::unordered_map<CVKPipelineGraphics*, CVKDescriptorSet*>>> m_pInputAttachmentDescriptorSets;

private:
	CVKDevice* m_pDevice;
};
