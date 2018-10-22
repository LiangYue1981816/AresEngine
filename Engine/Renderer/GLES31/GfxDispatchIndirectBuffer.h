#pragma once
#include "GfxRenderer.h"


class CGfxDispatchIndirectBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxDispatchIndirectBufferManager;


private:
	CGfxDispatchIndirectBuffer(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);
	virtual ~CGfxDispatchIndirectBuffer(void);
	virtual void Release(void);


public:
	void Bind(void) const;


private:
	uint32_t m_buffer;
};
