#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSolution::SetRenderSolution(RenderSolution solution)
{
	m_solution = solution;
}

void CRenderSolution::SetEnableMSAA(bool bEnable)
{
	m_bEnableMSAA = bEnable;
}

void CRenderSolution::Render(int indexQueue)
{
	m_pEngineUniform->Apply();
	m_pMainCameraUniform->Apply();
	m_pShadowCameraUniform[0]->Apply();
	m_pShadowCameraUniform[1]->Apply();
	m_pShadowCameraUniform[2]->Apply();
	m_pShadowCameraUniform[3]->Apply();

	switch (m_solution) {
	case RENDER_SOLUTION_DEFAULT:
		RenderDefault(indexQueue);
		break;

	case RENDER_SOLUTION_FORWARD:
		RenderForward(indexQueue);
		break;

	case RENDER_SOLUTION_DEFERRED:
		RenderDeferred(indexQueue);
		break;

	case RENDER_SOLUTION_TILED_BASE_DEFERRED:
		RenderTiledBaseDeferred(indexQueue);
		break;
	}
}

void CRenderSolution::RenderDefault(int indexQueue)
{
	GfxRenderer()->AcquireNextFrame();
	{
		m_pPassDefault->Render(indexQueue, m_bEnableMSAA);
	}
	GfxRenderer()->Present();
}

void CRenderSolution::RenderForward(int indexQueue)
{
	GfxRenderer()->AcquireNextFrame();
	{
		m_pPassShadow->Render(indexQueue);
		m_pPassForwardLighting->Render(indexQueue, m_bEnableMSAA);
	}
	GfxRenderer()->Present();
}

void CRenderSolution::RenderDeferred(int indexQueue)
{
	GfxRenderer()->AcquireNextFrame();
	{

	}
	GfxRenderer()->Present();
}

void CRenderSolution::RenderTiledBaseDeferred(int indexQueue)
{
	GfxRenderer()->AcquireNextFrame();
	{

	}
	GfxRenderer()->Present();
}
