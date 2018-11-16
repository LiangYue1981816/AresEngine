#include "Engine.h"
#include "RenderSolutionForward.h"


CRenderSolutionForward::CRenderSolutionForward(void)
{

}

CRenderSolutionForward::~CRenderSolutionForward(void)
{
	Clearup(0);
	Clearup(1);
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

void CRenderSolutionForward::SetEnableMSAA(bool bEnable, int samples)
{

}

void CRenderSolutionForward::Render(int indexQueue, float deltaTime)
{

}

void CRenderSolutionForward::Present(int indexQueue)
{

}

void CRenderSolutionForward::Clearup(int indexQueue)
{

}
