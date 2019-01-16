#include "GLES3Renderer.h"


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

CGLES3RenderPass* CGLES3RenderPassManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto &itRenderPass = m_pRenderPasses.find(name);

		if (itRenderPass != m_pRenderPasses.end()) {
			return itRenderPass->second;
		}
		else {
			return nullptr;
		}
	}
}

CGLES3RenderPass* CGLES3RenderPassManager::Create(uint32_t name, int numAttachments, int numSubpasses)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pRenderPasses[name] == nullptr) {
			m_pRenderPasses[name] = new CGLES3RenderPass(this, name, numAttachments, numSubpasses);
		}

		return m_pRenderPasses[name];
	}
}

void CGLES3RenderPassManager::Destroy(CGLES3RenderPass *pRenderPass)
{
	mutex_autolock autolock(&lock);
	{
		if (pRenderPass) {
			m_pRenderPasses.erase(pRenderPass->GetName());
			delete pRenderPass;
		}
	}
}
