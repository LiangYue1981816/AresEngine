#include "GfxHeader.h"


CGLES3RenderPassManager::CGLES3RenderPassManager(void)
{

}

CGLES3RenderPassManager::~CGLES3RenderPassManager(void)
{
	for (const auto &itRenderPasses : m_pRenderPasses) {
		delete itRenderPasses.second;
	}

	m_pRenderPasses.clear();
}

CGLES3RenderPass* CGLES3RenderPassManager::CreateRenderPass(int numAttachments, int numSubpasses)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3RenderPass *pRenderPass = new CGLES3RenderPass(this, numAttachments, numSubpasses);
		m_pRenderPasses[pRenderPass] = pRenderPass;
		return pRenderPass;
	}
}

void CGLES3RenderPassManager::DestroyRenderPass(CGLES3RenderPass *pRenderPass)
{
	mutex_autolock autolock(&lock);
	{
		if (pRenderPass) {
			m_pRenderPasses.erase(pRenderPass);
			delete pRenderPass;
		}
	}
}
