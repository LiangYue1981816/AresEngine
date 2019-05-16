#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawInstance : public CGfxCommandBase
{
public:
	CVKCommandDrawInstance(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_vkCommandBuffer(vkCommandBuffer)
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
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			if (m_ptrMeshDraw) {
				((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->Bind(m_vkCommandBuffer);
				vkCmdDrawIndexed(m_vkCommandBuffer, m_ptrMeshDraw->GetIndexCount(), m_ptrMeshDraw->GetInstanceCount(), m_ptrMeshDraw->GetIndexFirst(), 0, 0);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
