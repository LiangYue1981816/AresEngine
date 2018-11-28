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
	CGfxRenderPass* Create(int numAttachments, int numSubpasses);
	void Destroy(CGfxRenderPass *pRenderPass);


private:
	eastl::unordered_map<CGfxRenderPass*, CGfxRenderPass*> m_pRenderPasses;
};
