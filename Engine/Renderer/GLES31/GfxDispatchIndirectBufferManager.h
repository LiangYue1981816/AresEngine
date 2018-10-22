#pragma once
#include "GfxRenderer.h"


class CGfxDispatchIndirectBufferManager
{
	friend class CGfxRenderer;


private:
	CGfxDispatchIndirectBufferManager(void);
	virtual ~CGfxDispatchIndirectBufferManager(void);


private:
	CGfxDispatchIndirectBuffer* CreateDispatchIndirectBuffer(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);
	void DestroyDispatchIndirectBuffer(CGfxDispatchIndirectBuffer *pBuffer);


private:
	eastl::unordered_map<CGfxDispatchIndirectBuffer*, CGfxDispatchIndirectBuffer*> m_pBuffers;
};
