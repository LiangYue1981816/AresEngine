#include "EngineHeader.h"


const CGfxSemaphore* CCamera::RenderDefault(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bPresent) const
{
	m_pPassDefault->Update();
	return m_pPassDefault->Render(taskGraph, pWaitSemaphore, bPresent);
}

const CGfxSemaphore* CCamera::RenderForwardLighting(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bShadow, bool bPresent) const
{
	if (bShadow) {
		m_pPassShadow->Update();
		pWaitSemaphore = m_pPassShadow->Render(taskGraph, pWaitSemaphore);
	}

	m_pPassForwardLighting->Update();
	return m_pPassForwardLighting->Render(taskGraph, pWaitSemaphore, bPresent);
}
