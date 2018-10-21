#include "TaskCommandBuffer.h"
#include "RenderSolutionForward.h"


CRenderSolutionForward::CRenderSolutionForward(void)
{

}

CRenderSolutionForward::~CRenderSolutionForward(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionForward::SetEnableMSAA(bool bEnable, int samples)
{

}

void CRenderSolutionForward::CreateFrameBuffer(void)
{

}

void CRenderSolutionForward::DestroyFrameBuffer(void)
{

}

void CRenderSolutionForward::CreateFrameBufferMSAA(int samples)
{

}

void CRenderSolutionForward::DestroyFrameBufferMSAA(void)
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
