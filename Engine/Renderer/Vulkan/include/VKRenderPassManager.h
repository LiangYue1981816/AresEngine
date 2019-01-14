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
	CVKRenderPass* Create(int numAttachments, int numSubpasses);
	void Destroy(CGfxRenderPass *pRenderPass);


private:
	eastl::unordered_map<CGfxRenderPass*, CGfxRenderPass*> m_pRenderPasses;

private:
	CVKDevice *m_pDevice;
};
