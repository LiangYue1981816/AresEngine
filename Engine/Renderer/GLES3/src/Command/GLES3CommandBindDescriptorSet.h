#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindDescriptorSet : public CGfxCommandBase
{
public:
	CGLES3CommandBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
		: m_ptrDescriptorSet(ptrDescriptorSet)
	{

	}
	virtual ~CGLES3CommandBindDescriptorSet(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET, "CommandBindDescriptorSet");
		{
			if (m_ptrDescriptorSet) {
				GLES3Renderer()->BindDescriptorSet(m_ptrDescriptorSet);
			}
		}
	}


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
};
