#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawInstance : public CGfxCommandBase
{
public:
	CVKCommandDrawInstance(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_ptrMeshDraw(ptrMeshDraw)
	{
		Execute();
	}
	virtual ~CVKCommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrMeshDraw);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
				m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
				vkCmdDrawIndexed(m_vkCommandBuffer, m_ptrMeshDraw->GetIndexCount(), m_ptrMeshDraw->GetInstanceCount(), m_ptrMeshDraw->GetIndexFirst(), 0, 0);
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
