#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxRenderPassManager.h"


CGfxRenderPassManager::CGfxRenderPassManager(void)
{

}

CGfxRenderPassManager::~CGfxRenderPassManager(void)
{
	for (const auto &itRenderPasses : m_pRenderPasses) {
		delete itRenderPasses.second;
	}

	m_pRenderPasses.clear();
}

CGfxRenderPass* CGfxRenderPassManager::CreateRenderPass(uint32_t numAttachments, uint32_t numSubpasses)
{
	CGfxRenderPass *pRenderPass = new CGfxRenderPass(numAttachments, numSubpasses);
	m_pRenderPasses[pRenderPass] = pRenderPass;
	return m_pRenderPasses[pRenderPass];
}

void CGfxRenderPassManager::DestroyRenderPass(CGfxRenderPass *pRenderPass)
{
	if (pRenderPass) {
		m_pRenderPasses.erase(pRenderPass);
		delete pRenderPass;
	}
}
