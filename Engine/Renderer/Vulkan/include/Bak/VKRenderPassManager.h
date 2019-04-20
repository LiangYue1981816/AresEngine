#pragma once
#include "VKRenderer.h"


class CVKRenderPassManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKRenderPass;


private:
	CVKRenderPassManager(CVKDevice *pDevice);
	virtual ~CVKRenderPassManager(void);


private:
	CVKRenderPass* Get(uint32_t name);
	CVKRenderPass* Create(uint32_t name, int numAttachments, int numSubpasses);
	void Destroy(CVKRenderPass *pRenderPass);


private:
	eastl::unordered_map<uint32_t, CVKRenderPass*> m_pRenderPasses;

private:
	CVKDevice *m_pDevice;
};
