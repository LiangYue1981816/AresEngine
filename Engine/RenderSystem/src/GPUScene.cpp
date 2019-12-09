#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUScene::CGPUScene(void)
{

}

CGPUScene::~CGPUScene(void)
{

}

uint32_t CGPUScene::AddInstance(void)
{
	uint32_t index;

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

void CGPUScene::RemoveInstance(uint32_t index)
{
	if (index >= 0 && index < m_instanceBuffer.size()) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			m_freeIndex.emplace(index);
			m_transferBuffer[Engine()->GetFrameCount() % 2].erase(index);
		}
	}
}

void CGPUScene::ModifyInstanceData(uint32_t index, const InstanceData &data)
{
	if (index >= 0 && index < m_instanceBuffer.size()) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			m_transferBuffer[Engine()->GetFrameCount() % 2][index] = TransferData(index, data);
		}
	}
}

const CGPUScene::InstanceData& CGPUScene::GetInstanceData(uint32_t index) const
{
	static InstanceData invalid;

	if (index >= 0 && index < m_instanceBuffer.size()) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			return m_instanceBuffer[index];
		}
	}

	return invalid;
}

void CGPUScene::Update(void)
{
	for (const auto& itTransfer : m_transferBuffer[Engine()->GetFrameCount() % 2]) {
		m_instanceBuffer[itTransfer.second.index] = itTransfer.second.data;
	}

	m_transferBuffer[Engine()->GetFrameCount() % 2].clear();
}
