#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDrawIndirectBufferManager.h"


CGfxDrawIndirectBufferManager::CGfxDrawIndirectBufferManager(void)
{

}

CGfxDrawIndirectBufferManager::~CGfxDrawIndirectBufferManager(void)
{
	for (const auto &itBuffer : m_pBuffers) {
		delete itBuffer.second;
	}

	m_pBuffers.clear();
}

CGfxDrawIndirectBuffer* CGfxDrawIndirectBufferManager::CreateDrawIndirectBuffer(const CGfxMesh *pMesh, int baseVertex, int firstIndex, int indexCount)
{
	char szName[_MAX_STRING];
	sprintf(szName, "%8.8X_%8.8X_%8.8X_%8.8X", pMesh->GetName(), baseVertex, firstIndex, indexCount);

	uint32_t name = HashValue(szName);

	if (m_pBuffers[name] == NULL) {
		m_pBuffers[name] = new CGfxDrawIndirectBuffer(name, baseVertex, firstIndex, indexCount);
	}

	return m_pBuffers[name];
}

void CGfxDrawIndirectBufferManager::DestroyDrawIndirectBuffer(CGfxDrawIndirectBuffer *pBuffer)
{
	if (pBuffer) {
		m_pBuffers.erase(pBuffer->GetName());
		delete pBuffer;
	}
}
