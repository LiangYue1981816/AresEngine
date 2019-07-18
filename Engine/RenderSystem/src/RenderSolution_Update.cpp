#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSolution::UpdateCamera(int indexQueue)
{
	m_pMainCameraQueue->Clear(indexQueue);
	SceneManager()->UpdateCamera(m_pMainCamera, m_pMainCameraQueue, indexQueue);

	switch (m_solution) {
	case RENDER_SOLUTION_DEFAULT:
		UpdateDefault();
		break;

	case RENDER_SOLUTION_FORWARD:
		UpdateForward();
		break;

	case RENDER_SOLUTION_DEFERRED:
		UpdateDeferred();
		break;

	case RENDER_SOLUTION_TILED_BASE_DEFERRED:
		UpdateTiledBaseDeferred();
		break;
	}
}

void CRenderSolution::UpdateDefault(void)
{
	m_pPassDefault->Update();
}

void CRenderSolution::UpdateForward(void)
{
	m_pPassShadow->Update();
	m_pPassForwardLighting->Update();
}

void CRenderSolution::UpdateDeferred(void)
{

}

void CRenderSolution::UpdateTiledBaseDeferred(void)
{

}
