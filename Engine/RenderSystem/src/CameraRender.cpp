#include "EngineHeader.h"


const CGfxSemaphore* CCamera::RenderDefault(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bPresent) const
{
	m_pPassDefault->Update();
	m_pPassDefault->Render(taskGraph, pWaitSemaphore, bPresent);

	return m_pPassDefault->GetSemaphore();
}

const CGfxSemaphore* CCamera::RenderForwardLighting(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bShadow, bool bPresent) const
{
	m_pPassForwardLighting->Update();
	m_pPassForwardLighting->Render(taskGraph, pWaitSemaphore, bPresent);

	return m_pPassForwardLighting->GetSemaphore();
}
