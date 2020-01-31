#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindInstanceBuffer : public CGfxCommandBase
{
public:
	CVKCommandBindInstanceBuffer(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxInstanceBufferPtr ptrInstanceBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_ptrInstanceBuffer(ptrInstanceBuffer)
	{

	}
	virtual ~CVKCommandBindInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrInstanceBuffer);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_INSTANCEBUFFER, "CommandBindInstanceBuffer");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrInstanceBuffer->GetInstanceBinding(), m_ptrInstanceBuffer->GetInstanceFormat())) {
				((CVKInstanceBuffer*)m_ptrInstanceBuffer.GetPointer())->Bind(m_vkCommandBuffer);
			}
		}
	}


private:
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;

private:
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
