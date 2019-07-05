#pragma once
#include "GLES3Renderer.h"


class CGLES3DescriptorSetManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3DescriptorSet;


private:
	CGLES3DescriptorSetManager(void);
	virtual ~CGLES3DescriptorSetManager(void);


private:
	CGLES3DescriptorSet* Get(uint32_t name);
	CGLES3DescriptorSet* Create(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CGLES3DescriptorSet* Create(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom);
	CGLES3DescriptorSet* Create(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass);
	void Destroy(CGLES3DescriptorSet* pDescriptorSet);


private:
	eastl::unordered_map<uint32_t, CGLES3DescriptorSet*> m_pDescriptorSets;
	eastl::unordered_map<CGLES3FrameBuffer*, eastl::unordered_map<SubpassInformation*, eastl::unordered_map<CGLES3PipelineGraphics*, CGLES3DescriptorSet*>>> m_pInputAttachmentDescriptorSets;
};
