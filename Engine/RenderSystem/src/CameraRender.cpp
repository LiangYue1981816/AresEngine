#include "EngineHeader.h"


const CGfxSemaphore* CCamera::RenderDefault(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore) const
{
	m_pPassDefault->Update();
	m_pPassDefault->Render(taskGraph, pWaitSemaphore);

	return m_pPassDefault->GetSemaphore();
}

const CGfxSemaphore* CCamera::RenderForwardLighting(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bShadow) const
{
	m_pPassForwardLighting->Update();
	m_pPassForwardLighting->Render(taskGraph, pWaitSemaphore);

	return m_pPassForwardLighting->GetSemaphore();
}
