#include "EngineHeader.h"


CRenderSolutionDeferred::CRenderSolutionDeferred(void)
{

}

CRenderSolutionDeferred::~CRenderSolutionDeferred(void)
{
	Clearup(0);
	Clearup(1);
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

void CRenderSolutionDeferred::SetEnableMSAA(bool bEnable, int samples)
{

}

void CRenderSolutionDeferred::Render(int indexQueue)
{

}

void CRenderSolutionDeferred::Present(int indexQueue)
{

}

void CRenderSolutionDeferred::Clearup(int indexQueue)
{

}
