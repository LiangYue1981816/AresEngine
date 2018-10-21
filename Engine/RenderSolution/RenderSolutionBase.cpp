#include "RenderSolutionBase.h"


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_taskCommandBuffer("TashGraph_CommandBuffer", 50)
	, m_mainCommandBuffer{ CGfxCommandBuffer(true), CGfxCommandBuffer(true) }
{

}

CRenderSolutionBase::~CRenderSolutionBase(void)
{

}
