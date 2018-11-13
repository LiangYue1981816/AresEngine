#include "Engine.h"
#include "RenderSolutionDeferred.h"


CRenderSolutionDeferred::CRenderSolutionDeferred(void)
{

}

CRenderSolutionDeferred::~CRenderSolutionDeferred(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionDeferred::SetEnableMSAA(bool bEnable, int samples)
{

}

void CRenderSolutionDeferred::CreateFrameBuffer(void)
{

}

void CRenderSolutionDeferred::DestroyFrameBuffer(void)
{

}

void CRenderSolutionDeferred::CreateFrameBufferMSAA(int samples)
{

}

void CRenderSolutionDeferred::DestroyFrameBufferMSAA(void)
{

}

void CRenderSolutionDeferred::Render(int indexQueue)
{

}

void CRenderSolutionDeferred::RenderThread(void)
{

}

void CRenderSolutionDeferred::Present(int indexQueue)
{

}

void CRenderSolutionDeferred::Clearup(int indexQueue)
{

}
