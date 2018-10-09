#include "RenderSolutionBase.h"


CRenderSolutionBase::CRenderSolutionBase(int screenWidth, int screenHeight)
	: m_taskCommandBuffer("TashGraph_CommandBuffer", 50)
	, m_mainCommandBuffer{ CGfxCommandBuffer(true), CGfxCommandBuffer(true) }
{
	m_ptrFrameBufferScreen = Renderer()->CreateFrameBuffer(screenWidth, screenHeight);
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{

}
