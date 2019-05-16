#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawIndirect : public CGfxCommandBase
{
public:
	CVKCommandDrawIndirect(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_ptrMeshDraw(ptrMeshDraw)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (m_ptrMeshDraw) {

			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
