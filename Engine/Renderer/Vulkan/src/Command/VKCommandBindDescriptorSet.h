#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindDescriptorSet : public CGfxCommandBase
{
public:
	CVKCommandBindDescriptorSet(VkCommandBuffer vkCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
		: m_ptrDescriptorSet(ptrDescriptorSet)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandBindDescriptorSet(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET, "CommandBindDescriptorSet");
		{
			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				((CVKDescriptorSet*)m_ptrDescriptorSet.GetPointer())->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipelineLayout());
			}
		}
	}


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
};
