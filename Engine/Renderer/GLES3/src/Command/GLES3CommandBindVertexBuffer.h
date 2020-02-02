#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindVertexBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandBindVertexBuffer(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxVertexBufferPtr ptrVertexBuffer)
		: m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrVertexBuffer(ptrVertexBuffer)
	{

	}
	virtual ~CGLES3CommandBindVertexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrVertexBuffer);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_VERTEXBUFFER);
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrVertexBuffer->GetVertexBinding(), m_ptrVertexBuffer->GetVertexFormat())) {
				((CGLES3VertexBuffer*)m_ptrVertexBuffer.GetPointer())->Bind();
			}
		}
	}


private:
	CGfxVertexBufferPtr m_ptrVertexBuffer;

private:
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
