#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawInstance : public CGfxCommandBase
{
public:
	CVKCommandDrawInstance(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_ptrMeshDraw(ptrMeshDraw)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			if (m_ptrMeshDraw) {
				((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->Bind(vkCommandBuffer);
				vkCmdDrawIndexed(vkCommandBuffer, m_ptrMeshDraw->GetIndexCount(), m_ptrMeshDraw->GetInstanceCount(), m_ptrMeshDraw->GetIndexFirst(), 0, 0);
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
