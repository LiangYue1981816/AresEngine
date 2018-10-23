#pragma once
#include "GfxRenderer.h"


class CGfxDrawIndirectBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxDrawIndirectBufferManager;


private:
	CGfxDrawIndirectBuffer(uint32_t name, int baseVertex, uint32_t firstIndex, uint32_t indexCount);
	virtual ~CGfxDrawIndirectBuffer(void);
	virtual void Release(void);

public:
	uint32_t GetName(void) const;


public:
	void Bind(uint32_t instanceCount);


private:
	uint32_t m_name;

private:
	uint32_t m_buffer;

private:
	int m_baseVertex;
	uint32_t m_firstIndex;
	uint32_t m_indexCount;
	uint32_t m_instanceCount;
};
