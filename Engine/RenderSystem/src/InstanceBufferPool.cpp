#include "EngineHeader.h"
#include "RenderHeader.h"


CInstanceBufferPool::CInstanceBufferPool(void)
{

}

CInstanceBufferPool::~CInstanceBufferPool(void)
{

}

void CInstanceBufferPool::Clear(void)
{
	m_indexMultiInstanceBuffers.clear();
}

CGfxMultiInstanceBufferPtr CInstanceBufferPool::GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count)
{
	int& index = m_indexMultiInstanceBuffers[instanceFormat][instanceBinding][count];
	eastl::vector<CGfxMultiInstanceBufferPtr>& buffers = m_ptrMultiInstanceBuffers[instanceFormat][instanceBinding][count];

	if (index == buffers.size()) {
		buffers.emplace_back(GfxRenderer()->NewMultiInstanceBuffer(instanceFormat, instanceBinding, count));
	}

	return buffers[index++];
}
