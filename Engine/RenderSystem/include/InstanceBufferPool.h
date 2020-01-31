#pragma once
#include "PreHeader.h"


class CALL_API CInstanceBufferPool
{
public:
	CInstanceBufferPool(void);
	virtual ~CInstanceBufferPool(void);


public:
	void Clear(void);
	CGfxMultiInstanceBufferPtr GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding = 1, int count = 3);


private:
	eastl::unordered_map<uint32_t, eastl::unordered_map<int, eastl::unordered_map<int, int>>> m_indexMultiInstanceBuffers; // [format][binding][count][index]
	eastl::unordered_map<uint32_t, eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<CGfxMultiInstanceBufferPtr>>>> m_ptrMultiInstanceBuffers; // [format][binding][count][vector]
};
