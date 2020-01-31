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

	}
	virtual ~CVKCommandBindVertexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrVertexBuffer);
		ASSERT(m_pPipelineGraphics);
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_VERTEXBUFFER, "CommandBindVertexBuffer");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrVertexBuffer->GetVertexBinding(), m_ptrVertexBuffer->GetVertexFormat())) {
				((CVKVertexBuffer*)m_ptrVertexBuffer.GetPointer())->Bind(m_vkCommandBuffer);
			}
		}
	}


private:
	CGfxVertexBufferPtr m_ptrVertexBuffer;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
