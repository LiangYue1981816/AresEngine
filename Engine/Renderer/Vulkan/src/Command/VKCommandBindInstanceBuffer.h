#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindInstanceBuffer : public CGfxCommandBase
{
public:
	CVKCommandBindInstanceBuffer(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxInstanceBufferPtr ptrInstanceBuffer, int offset)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_ptrInstanceBuffer(ptrInstanceBuffer)
		, m_offset(offset)
	{
		Execute();
	}
	virtual ~CVKCommandBindInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrInstanceBuffer);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_INSTANCEBUFFER);
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrInstanceBuffer->GetInstanceBinding(), m_ptrInstanceBuffer->GetInstanceFormat())) {
				vkCmdBindVertexBuffer(m_vkCommandBuffer, m_ptrInstanceBuffer->GetInstanceBinding(), ((CVKInstanceBuffer*)m_ptrInstanceBuffer.GetPointer())->GetBuffer(), m_offset);
			}
		}
	}


private:
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;
	int m_offset;

private:
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
