#pragma once
#include "GfxRenderer.h"


class CGfxDrawIndirectBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxDrawIndirectBufferManager;


private:
	CGfxDrawIndirectBuffer(uint32_t name, int baseVertex, int firstIndex, int indexCount);
	virtual ~CGfxDrawIndirectBuffer(void);
	virtual void Release(void);

public:
	uint32_t GetName(void) const;


public:
	int GetBaseVertex(void) const;
	int GetFirstIndex(void) const;
	int GetIndexCount(void) const;
	int GetInstanceCount(void) const;

public:
	void Bind(int instanceCount);


private:
	uint32_t m_name;

private:
	uint32_t m_buffer;

private:
	int m_baseVertex;
	int m_firstIndex;
	int m_indexCount;
	int m_instanceCount;
};
