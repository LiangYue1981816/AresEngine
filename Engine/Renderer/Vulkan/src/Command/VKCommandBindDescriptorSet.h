#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindDescriptorSet : public CGfxCommandBase
{
public:
	CVKCommandBindDescriptorSet(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxDescriptorSetPtr ptrDescriptorSet)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
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
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET, "CommandBindDescriptorSet");
		{
			if (m_ptrDescriptorSet) {
				if (m_pPipelineCompute &&
					m_pPipelineCompute->GetDescriptorLayout(m_ptrDescriptorSet->GetDescriptorLayout()->GetSetIndex())->IsCompatible(m_ptrDescriptorSet->GetDescriptorLayout())) {
					((CVKDescriptorSet*)m_ptrDescriptorSet.GetPointer())->Bind(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_pPipelineCompute->GetPipelineLayout());
				}

				if (m_pPipelineGraphics &&
					m_pPipelineGraphics->GetDescriptorLayout(m_ptrDescriptorSet->GetDescriptorLayout()->GetSetIndex())->IsCompatible(m_ptrDescriptorSet->GetDescriptorLayout())) {
					((CVKDescriptorSet*)m_ptrDescriptorSet.GetPointer())->Bind(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelineGraphics->GetPipelineLayout());
				}
			}
		}
	}


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
