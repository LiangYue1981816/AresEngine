#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawIndirect : public CGfxCommandBase
{
public:
	CVKCommandDrawIndirect(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_ptrMeshDraw(ptrMeshDraw)
	{
		Execute();
	}
	virtual ~CVKCommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineGraphics);
		ASSERT(m_ptrMeshDraw);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
				m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
				((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->Bind(m_vkCommandBuffer);
				vkCmdDrawIndexedIndirect(m_vkCommandBuffer, ((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->GetIndirectBuffer(), 0, 1, ((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->GetIndirectBufferStride());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
