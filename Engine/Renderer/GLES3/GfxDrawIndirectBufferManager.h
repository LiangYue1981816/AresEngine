#pragma once
#include "GfxRenderer.h"


class CGfxDrawIndirectBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxDrawIndirectBufferManager(void);
	virtual ~CGfxDrawIndirectBufferManager(void);


private:
	CGfxDrawIndirectBuffer* CreateDrawIndirectBuffer(const CGfxMesh *pMesh, int baseVertex, int firstIndex, int indexCount);
	void DestroyDrawIndirectBuffer(CGfxDrawIndirectBuffer *pBuffer);


private:
	eastl::unordered_map<uint32_t, CGfxDrawIndirectBuffer*> m_pBuffers;
};
