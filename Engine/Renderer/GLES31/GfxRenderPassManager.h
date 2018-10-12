#pragma once
#include "GfxRenderer.h"


class CGfxRenderPassManager
{
	friend class CGfxRenderer;


private:
	CGfxRenderPassManager(void);
	virtual ~CGfxRenderPassManager(void);


private:
	CGfxRenderPass* CreateRenderPass(uint32_t numAttachments, uint32_t numSubpasses);
	void DestroyRenderPass(CGfxRenderPass *pRenderPass);


private:
	eastl::unordered_map<CGfxRenderPass*, CGfxRenderPass*> m_pRenderPasses;
};
