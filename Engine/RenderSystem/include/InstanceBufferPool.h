#pragma once
#include "PreHeader.h"


class CInstanceBufferPool
{
public:
	CInstanceBufferPool(void);
	virtual ~CInstanceBufferPool(void);


public:
	void Clear(void);
	CGfxInstanceBufferPtr GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding);


private:
	eastl::unordered_map<uint32_t, eastl::unordered_map<int, int>> m_indexInstanceBuffers; // [format][binding][index]
	eastl::unordered_map<uint32_t, eastl::unordered_map<int, eastl::vector<CGfxInstanceBufferPtr>>> m_ptrInstanceBuffers; // [format][binding][vector]
};
