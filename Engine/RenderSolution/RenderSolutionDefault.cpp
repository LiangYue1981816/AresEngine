#include "RenderSolutionDefault.h"
#include "TaskCommandBuffer.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
{

}

CRenderSolutionDefault::~CRenderSolutionDefault(void)
{

}

void CRenderSolutionDefault::Render(void)
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

void CRenderSolutionDefault::Present(void)
{
	MainCamera()->Submit();
	Renderer()->Present();
}
