#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindVertexBuffer : public CGfxCommandBase
{
public:
	CVKCommandBindVertexBuffer(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxVertexBufferPtr ptrVertexBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_ptrVertexBuffer(ptrVertexBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandBindVertexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrVertexBuffer);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_VERTEXBUFFER);
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrVertexBuffer->GetVertexBinding(), m_ptrVertexBuffer->GetVertexFormat())) {
				((CVKVertexBuffer*)m_ptrVertexBuffer.GetPointer())->Bind(m_vkCommandBuffer);
			}
		}
	}


private:
	CGfxVertexBufferPtr m_ptrVertexBuffer;

private:
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
