#include "EngineHeader.h"


void CCamera::Update(CTaskGraph& taskGraph) const
{
	SceneManager()->UpdateCamera(taskGraph, m_pCamera, m_pRenderQueue, 0xffffffff);
}

const CGfxSemaphore* CCamera::RenderDefault(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bPresent) const
{
	return m_pPassDefault->Render(taskGraph, pWaitSemaphore, bPresent);
}

const CGfxSemaphore* CCamera::RenderForwardLighting(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bShadow, bool bPresent) const
{
	if (bShadow) {
		pWaitSemaphore = m_pPassShadow->Render(taskGraph, pWaitSemaphore);
	}

	return m_pPassForwardLighting->Render(taskGraph, pWaitSemaphore, bPresent);
}
