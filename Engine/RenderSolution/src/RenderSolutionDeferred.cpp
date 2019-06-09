#include "EngineHeader.h"


CRenderSolutionDeferred::CRenderSolutionDeferred(CRenderSolution* pRenderSolution)
	: CRenderSolutionBase(pRenderSolution)
{

}

CRenderSolutionDeferred::~CRenderSolutionDeferred(void)
{
	Destroy();
}

void CRenderSolutionDeferred::CreateDescriptorSet(void)
{

}

void CRenderSolutionDeferred::DestroyDescriptorSet(void)
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

void CRenderSolutionDeferred::Create(int samples)
{

}

void CRenderSolutionDeferred::Destroy(void)
{

}

void CRenderSolutionDeferred::UpdateCamera(int indexQueue)
{

}

void CRenderSolutionDeferred::Render(int indexQueue)
{

}
