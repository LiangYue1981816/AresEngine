#pragma once
#include "GfxRenderer.h"


class CGfxDrawIndirectBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxDrawIndirectBufferManager(void);
	virtual ~CGfxDrawIndirectBufferManager(void);


private:
	CGfxDrawIndirectBuffer* CreateDrawIndirectBuffer(const CGfxMesh *pMesh, int baseVertex, uint32_t firstIndex, uint32_t indexCount);
	void DestroyDrawIndirectBuffer(CGfxDrawIndirectBuffer *pBuffer);


private:
	eastl::unordered_map<uint32_t, CGfxDrawIndirectBuffer*> m_pBuffers;
};
