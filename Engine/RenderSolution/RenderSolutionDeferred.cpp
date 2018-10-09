#include "TaskCommandBuffer.h"
#include "RenderSolutionDeferred.h"


CRenderSolutionDeferred::CRenderSolutionDeferred(int screenWidth, int screenHeight)
	: CRenderSolutionBase(screenWidth, screenHeight)
{

}

CRenderSolutionDeferred::~CRenderSolutionDeferred(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionDeferred::SetEnableMSAA(bool bEnable, int width, int height, int samples)
{

}

void CRenderSolutionDeferred::Render(int indexQueue)
{
	m_taskCommandBuffer.Wait();
	{
		static uint32_t namePass = HashValue("Default");
		static CTaskCommandBuffer taskCommandBuffers[THREAD_COUNT];

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskCommandBuffers[indexThread].SetParams(indexThread, indexQueue, namePass);
			m_taskCommandBuffer.Task(&taskCommandBuffers[indexThread], MainCamera(), NULL, NULL);
		}
	}
	m_taskCommandBuffer.Dispatch();
}

void CRenderSolutionDeferred::Present(int indexQueue)
{
	Renderer()->Present();
}

void CRenderSolutionDeferred::Clearup(int indexQueue)
{

}
