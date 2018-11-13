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
	CGLES3CommandBuffer* CreateCommandBuffer(bool bMainCommandBuffer);
	void DestroyCommandBuffer(CGLES3CommandBuffer *pCommandBuffer);


private:
	eastl::unordered_map<CGLES3CommandBuffer*, CGLES3CommandBuffer*> m_pCommandBuffers;
};
