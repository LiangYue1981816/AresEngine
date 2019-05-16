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
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (m_ptrMeshDraw) {
				((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->Bind(m_vkCommandBuffer);
				vkCmdDrawIndexedIndirect(m_vkCommandBuffer, ((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->GetIndirectBuffer(), 0, 1, ((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->GetIndirectBufferStride());
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
