#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindDescriptorSet : public CGfxCommandBase
{
public:
	CGLES3CommandBindDescriptorSet(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxDescriptorSetPtr ptrDescriptorSet)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrDescriptorSet(ptrDescriptorSet)
	{

	}
	virtual ~CGLES3CommandBindDescriptorSet(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET, "CommandBindDescriptorSet");
		{
			if (m_ptrDescriptorSet) {
				if (m_pPipelineCompute) {
					m_pPipelineCompute->BindDescriptorSet(m_ptrDescriptorSet);
				}

				if (m_pPipelineGraphics) {
					m_pPipelineGraphics->BindDescriptorSet(m_ptrDescriptorSet);
				}
			}
		}
	}


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
