#pragma once
#include "GLES3Renderer.h"


class CGLES3RenderPassManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3RenderPass;


private:
	CGLES3RenderPassManager(void);
	virtual ~CGLES3RenderPassManager(void);


private:
	CGLES3RenderPass* CreateRenderPass(int numAttachments, int numSubpasses);
	void DestroyRenderPass(CGLES3RenderPass *pRenderPass);


private:
	eastl::unordered_map<CGLES3RenderPass*, CGLES3RenderPass*> m_pRenderPasses;
};
