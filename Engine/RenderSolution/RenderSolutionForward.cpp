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

}

void CRenderSolutionForward::Present(int indexQueue)
{

}

void CRenderSolutionForward::Clearup(int indexQueue)
{

}
