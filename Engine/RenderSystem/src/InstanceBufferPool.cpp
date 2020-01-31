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

CGfxMultiInstanceBufferPtr CInstanceBufferPool::GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding)
{
	int& index = m_indexMultiInstanceBuffers[instanceFormat][instanceBinding];
	eastl::vector<CGfxMultiInstanceBufferPtr>& buffers = m_ptrMultiInstanceBuffers[instanceFormat][instanceBinding];

	if (index == buffers.size()) {
		buffers.emplace_back(GfxRenderer()->NewMultiInstanceBuffer(instanceFormat, instanceBinding, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT));
	}

	return buffers[index++];
}
