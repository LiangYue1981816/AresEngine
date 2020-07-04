#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::ComputeCluster(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera)
{
	m_pGPUCluster->SetCamera(pCamera);
	m_pGPUCluster->Compute(taskPool, taskGraph, ptrCommandBuffer);
}
