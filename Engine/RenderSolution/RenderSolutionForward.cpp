#include "TaskCommandBuffer.h"
#include "RenderSolutionForward.h"


CRenderSolutionForward::CRenderSolutionForward(int screenWidth, int screenHeight)
	: CRenderSolutionBase(screenWidth, screenHeight)
{

}

CRenderSolutionForward::~CRenderSolutionForward(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionForward::SetEnableMSAA(bool bEnable, int width, int height, int samples)
{

}

void CRenderSolutionForward::Render(int indexQueue)
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

void CRenderSolutionForward::Present(int indexQueue)
{
	Renderer()->Present();
}

void CRenderSolutionForward::Clearup(int indexQueue)
{

}
