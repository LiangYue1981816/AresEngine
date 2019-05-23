#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindDescriptorSet : public CGfxCommandBase
{
public:
	CVKCommandBindDescriptorSet(VkCommandBuffer vkCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrDescriptorSet(ptrDescriptorSet)
	{
		if (m_ptrDescriptorSet) {
			Execute();
		}
	}
	virtual ~CVKCommandBindDescriptorSet(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrDescriptorSet);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET, "CommandBindDescriptorSet");
		{
			VKRenderer()->BindDescriptorSet(m_vkCommandBuffer, m_ptrDescriptorSet);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
};
