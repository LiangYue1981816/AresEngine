#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSolution::UpdateCamera(int indexQueue)
{
	m_pMainQueue->Clear(indexQueue);
	m_pLightQueue->Clear(indexQueue);
	m_pShadowQueue->Clear(indexQueue);

	SceneManager()->UpdateCamera(m_pMainCamera, m_pMainQueue, indexQueue);
}
