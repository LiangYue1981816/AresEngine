#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindMeshDraw : public CGfxCommandBase
{
public:
	CVKCommandBindMeshDraw(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_ptrMeshDraw(ptrMeshDraw)
	{
		Execute();
	}
	virtual ~CVKCommandBindMeshDraw(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineGraphics);
		ASSERT(m_ptrMeshDraw);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESHDRAW, "CommandBindMeshDraw");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
				((CVKMeshDraw*)m_ptrMeshDraw.GetPointer())->Bind(m_vkCommandBuffer);
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
