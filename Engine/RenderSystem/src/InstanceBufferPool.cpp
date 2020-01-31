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
	return CGfxMultiInstanceBufferPtr(nullptr);
}
