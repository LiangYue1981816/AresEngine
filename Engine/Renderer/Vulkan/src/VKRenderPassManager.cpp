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

CVKRenderPass* CVKRenderPassManager::Create(int numAttachments, int numSubpasses)
{
	mutex_autolock autolock(&lock);
	{
		CVKRenderPass *pRenderPass = new CVKRenderPass(m_pDevice, this, numAttachments, numSubpasses);
		m_pRenderPasses[pRenderPass] = pRenderPass;
		return pRenderPass;
	}
}

void CVKRenderPassManager::Destroy(CGfxRenderPass *pRenderPass)
{
	mutex_autolock autolock(&lock);
	{
		if (pRenderPass) {
			m_pRenderPasses.erase(pRenderPass);
			delete pRenderPass;
		}
	}
}
