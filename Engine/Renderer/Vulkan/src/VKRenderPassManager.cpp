#include "VKRenderer.h"


CVKRenderPassManager::CVKRenderPassManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKRenderPassManager::~CVKRenderPassManager(void)
{
	for (const auto &itRenderPasses : m_pRenderPasses) {
		delete itRenderPasses.second;
	}

	m_pRenderPasses.clear();
}

CVKRenderPass* CVKRenderPassManager::Get(uint32_t name)
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

CVKRenderPass* CVKRenderPassManager::Create(uint32_t name, int numAttachments, int numSubpasses)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pRenderPasses[name] == nullptr) {
			m_pRenderPasses[name] = new CVKRenderPass(m_pDevice, this, name, numAttachments, numSubpasses);
		}

		return m_pRenderPasses[name];
	}
}

void CVKRenderPassManager::Destroy(CVKRenderPass *pRenderPass)
{
	mutex_autolock autolock(&lock);
	{
		if (pRenderPass) {
			m_pRenderPasses.erase(pRenderPass->GetName());
			delete pRenderPass;
		}
	}
}
