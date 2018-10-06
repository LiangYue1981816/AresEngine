#include "TaskCommandBuffer.h"
#include "RenderSolutionForward.h"


CRenderSolutionForward::CRenderSolutionForward(void)
{

}

CRenderSolutionForward::~CRenderSolutionForward(void)
{

}

void CRenderSolutionForward::Render(void)
{
	m_taskCommandBuffer.Wait();
	{
		static uint32_t namePass = HashValue("Default");
		static CTaskCommandBuffer taskCommandBuffers[THREAD_COUNT];

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskCommandBuffers[indexThread].SetParams(indexThread, namePass);
			m_taskCommandBuffer.Task(&taskCommandBuffers[indexThread], MainCamera(), NULL, NULL);
		}
	}
	m_taskCommandBuffer.Dispatch();
}

void CRenderSolutionForward::Present(void)
{
	MainCamera()->Submit();
	Renderer()->Present();
}
