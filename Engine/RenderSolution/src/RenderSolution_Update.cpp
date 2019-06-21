#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSolution::UpdateCamera(int indexQueue)
{
	m_pMainQueue->Clear(indexQueue);
	m_pShadowQueue->Clear(indexQueue);

	switch (m_solution) {
	case RENDER_SOLUTION_DEFAULT:
		UpdateDefault(indexQueue);
		break;

	case RENDER_SOLUTION_FORWARD:
		UpdateForward(indexQueue);
		break;

	case RENDER_SOLUTION_DEFERRED:
		UpdateDeferred(indexQueue);
		break;

	case RENDER_SOLUTION_TILED_BASE_DEFERRED:
		UpdateTiledBaseDeferred(indexQueue);
		break;
	}
}

void CRenderSolution::UpdateDefault(int indexQueue)
{
	m_pPassDefault->Update();

	SceneManager()->UpdateCamera(m_pMainCamera, m_pMainQueue, indexQueue);
}

void CRenderSolution::UpdateForward(int indexQueue)
{
	m_pPassShadow->Update();
	m_pPassForwardLighting->Update();

	SceneManager()->UpdateCamera(m_pMainCamera, m_pMainQueue, indexQueue);
	SceneManager()->UpdateCamera(m_pShadowCamera, m_pShadowQueue, indexQueue);
}

void CRenderSolution::UpdateDeferred(int indexQueue)
{

}

void CRenderSolution::UpdateTiledBaseDeferred(int indexQueue)
{

}
