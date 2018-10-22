#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDispatchIndirectBufferManager.h"


CGfxDispatchIndirectBufferManager::CGfxDispatchIndirectBufferManager(void)
{

}

CGfxDispatchIndirectBufferManager::~CGfxDispatchIndirectBufferManager(void)
{
	for (const auto &itBuffer : m_pBuffers) {
		delete itBuffer.second;
	}

	m_pBuffers.clear();
}

CGfxDispatchIndirectBuffer* CGfxDispatchIndirectBufferManager::CreateDispatchIndirectBuffer(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
{
	CGfxDispatchIndirectBuffer *pBuffer = new CGfxDispatchIndirectBuffer(numGroupsX, numGroupsY, numGroupsZ);
	m_pBuffers[pBuffer] = pBuffer;
	return m_pBuffers[pBuffer];
}

void CGfxDispatchIndirectBufferManager::DestroyDispatchIndirectBuffer(CGfxDispatchIndirectBuffer *pBuffer)
{
	if (pBuffer) {
		m_pBuffers.erase(pBuffer);
		delete pBuffer;
	}
}
