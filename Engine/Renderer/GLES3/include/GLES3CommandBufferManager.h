#pragma once
#include "GLES3Renderer.h"


class CGLES3CommandBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3CommandBuffer;


private:
	CGLES3CommandBufferManager(void);
	virtual ~CGLES3CommandBufferManager(void);


private:
	CGLES3CommandBuffer* Create(bool bMainCommandBuffer);
	void Destroy(CGfxCommandBuffer *pCommandBuffer);


private:
	eastl::unordered_map<CGfxCommandBuffer*, CGfxCommandBuffer*> m_pCommandBuffers;
};
