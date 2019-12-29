#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUScene::CGPUScene(void)
	: m_indexDefaultInstance(INVALID_VALUE)
	, m_indexPostProcessInstnace(INVALID_VALUE)
{
	m_indexDefaultInstance = AddInstance();
	m_indexPostProcessInstnace = AddInstance();

	m_ptrInstanceBuffer = GfxRenderer()->NewStorageBuffer(sizeof(InstanceData) * 16 * 1024);
	m_ptrTransferBuffer = GfxRenderer()->NewStorageBuffer(sizeof(TransferData) * 16 * 1024);
}

CGPUScene::~CGPUScene(void)
{

}

const CGfxStorageBufferPtr CGPUScene::GetInstanceBuffer(void) const
{
	return m_ptrInstanceBuffer;
}

const CGfxStorageBufferPtr CGPUScene::GetTransferBuffer(void) const
{
	return m_ptrTransferBuffer;
}

int CGPUScene::AddInstance(void)
{
	int index;

	if (m_freeIndex.empty()) {
		index = m_instanceBuffer.size();
		m_instanceBuffer.emplace_back();
	}
	else {
		index = *m_freeIndex.begin();
		m_freeIndex.erase(index);
	}

	return index;
}

void CGPUScene::RemoveInstance(int index)
{
	if (index >= 0 && index < m_instanceBuffer.size() && index != m_indexDefaultInstance) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			m_freeIndex.emplace(index);

			for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
				m_transferBuffer[indexThread].erase(index);
			}
		}
	}
}

void CGPUScene::ModifyInstanceData(int index, const InstanceData &data, int indexThread)
{
	if (indexThread >= 0 && indexThread < MAX_THREAD_COUNT) {
		if (index >= 0 && index < m_instanceBuffer.size() && index != m_indexDefaultInstance) {
			if (m_freeIndex.find(index) == m_freeIndex.end()) {
				m_transferBuffer[indexThread][index] = TransferData(index, data);
			}
		}
	}
}

int CGPUScene::GetDefaultInstanceIndex(void) const
{
	return m_indexDefaultInstance;
}

int CGPUScene::GetPostProcessInstnaceIndex(void) const
{
	return m_indexPostProcessInstnace;
}

const CGPUScene::InstanceData& CGPUScene::GetInstanceData(int index) const
{
	static InstanceData invalid;

	if (index >= 0 && index < m_instanceBuffer.size()) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			return m_instanceBuffer[index];
		}
	}

	return invalid;
}

void CGPUScene::Update(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	bool bNeedUpdate = false;

	for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
		for (const auto& itTransfer : m_transferBuffer[indexThread]) {
			m_instanceBuffer[itTransfer.second.index] = itTransfer.second.data;
			bNeedUpdate = true;
		}

		m_transferBuffer[indexThread].clear();
	}

	if (bNeedUpdate) {
		m_ptrInstanceBuffer->BufferData(0, sizeof(InstanceData) * m_instanceBuffer.size(), m_instanceBuffer.data());
	}
}
