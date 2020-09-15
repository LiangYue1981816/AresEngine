#include "RenderHeader.h"


CInstanceBufferPool::CInstanceBufferPool(void)
{

}

CInstanceBufferPool::~CInstanceBufferPool(void)
{

}

void CInstanceBufferPool::Clear(void)
{
	m_indexInstanceBuffers.clear();
}

CGfxInstanceBufferPtr CInstanceBufferPool::GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding)
{
	int& index = m_indexInstanceBuffers[instanceFormat][instanceBinding];
	eastl::vector<CGfxInstanceBufferPtr>& buffers = m_ptrInstanceBuffers[instanceFormat][instanceBinding];

	if (index == buffers.size()) {
		buffers.emplace_back(GfxRenderer()->NewInstanceBuffer(instanceFormat, instanceBinding));
	}

	return buffers[index++];
}
