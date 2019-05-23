#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawIndirect : public CGfxCommandBase
{
public:
	CVKCommandDrawIndirect(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_vkCommandBuffer(vkCommandBuffer)
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
		ASSERT(m_ptrMeshDraw);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				if (pPipeline->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
					pPipeline->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
					((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->Bind(m_vkCommandBuffer);
					vkCmdDrawIndexedIndirect(m_vkCommandBuffer, ((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->GetIndirectBuffer(), 0, 1, ((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->GetIndirectBufferStride());
				}
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
