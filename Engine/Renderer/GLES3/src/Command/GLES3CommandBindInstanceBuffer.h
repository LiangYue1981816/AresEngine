#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindInstanceBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandBindInstanceBuffer(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxInstanceBufferPtr ptrInstanceBuffer, int offset)
		: m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrInstanceBuffer(ptrInstanceBuffer)
		, m_offset(offset)
	{

	}
	virtual ~CGLES3CommandBindInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrInstanceBuffer);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_INSTANCEBUFFER);
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrInstanceBuffer->GetInstanceBinding(), m_ptrInstanceBuffer->GetInstanceFormat())) {
				((CGLES3InstanceBuffer*)m_ptrInstanceBuffer.GetPointer())->Bind(m_offset);
			}
		}
	}


private:
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;
	int m_offset;

private:
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
