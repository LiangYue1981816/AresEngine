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
	CGLES3RenderPass* Get(uint32_t name);
	CGLES3RenderPass* Create(uint32_t name, int numAttachments, int numSubpasses);
	void Destroy(CGLES3RenderPass *pRenderPass);


private:
	eastl::unordered_map<uint32_t, CGLES3RenderPass*> m_pRenderPasses;
};
