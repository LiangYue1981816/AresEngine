#include "TaskCommandBuffer.h"
#include "RenderSolutionForwardPlus.h"


CRenderSolutionForwardPlus::CRenderSolutionForwardPlus(void)
{

}

CRenderSolutionForwardPlus::~CRenderSolutionForwardPlus(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionForwardPlus::SetEnableMSAA(bool bEnable, int samples)
{

}

void CRenderSolutionForwardPlus::CreateFrameBuffer(void)
{

}

void CRenderSolutionForwardPlus::DestroyFrameBuffer(void)
{

}

void CRenderSolutionForwardPlus::CreateFrameBufferMSAA(int samples)
{

}

void CRenderSolutionForwardPlus::DestroyFrameBufferMSAA(void)
{

}

void CRenderSolutionForwardPlus::Render(int indexQueue)
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

void CRenderSolutionForwardPlus::Present(int indexQueue)
{
	Renderer()->Present();
}

void CRenderSolutionForwardPlus::Clearup(int indexQueue)
{

}
