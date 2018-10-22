#pragma once
#include "GfxRenderer.h"


class CGfxDrawIndirectBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxDrawIndirectBufferManager;


private:
	CGfxDrawIndirectBuffer(int baseVertex, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount);
	virtual ~CGfxDrawIndirectBuffer(void);
	virtual void Release(void);


public:
	void Bind(void) const;


private:
	uint32_t m_buffer;
};
