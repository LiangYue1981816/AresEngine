#pragma once
#include "GfxRenderer.h"


class CGfxDrawIndirectBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxDrawIndirectBufferManager(void);
	virtual ~CGfxDrawIndirectBufferManager(void);


private:
	CGfxDrawIndirectBuffer* CreateDrawIndirectBuffer(int baseVertex, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount);
	void DestroyDrawIndirectBuffer(CGfxDrawIndirectBuffer *pBuffer);


private:
	eastl::unordered_map<CGfxDrawIndirectBuffer*, CGfxDrawIndirectBuffer*> m_pBuffers;
};
