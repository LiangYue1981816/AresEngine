#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindDescriptorSet : public CGfxCommandBase
{
public:
	CVKCommandBindDescriptorSet(VkCommandBuffer vkCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrDescriptorSet(ptrDescriptorSet)
	{
		Execute();
	}
	virtual ~CVKCommandBindDescriptorSet(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET, "CommandBindDescriptorSet");
		{
			if (m_ptrDescriptorSet) {
				VKRenderer()->BindDescriptorSet(m_vkCommandBuffer, m_ptrDescriptorSet);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
};
