#pragma once
#include "GfxRenderer.h"


class CGfxRenderPassManager
{
	friend class CGfxRenderer;


private:
	CGfxRenderPassManager(void);
	virtual ~CGfxRenderPassManager(void);


private:
	CGfxRenderPass* CreateRenderPass(int numAttachments, int numSubpasses);
	void DestroyRenderPass(CGfxRenderPass *pRenderPass);


private:
	eastl::unordered_map<CGfxRenderPass*, CGfxRenderPass*> m_pRenderPasses;
};
