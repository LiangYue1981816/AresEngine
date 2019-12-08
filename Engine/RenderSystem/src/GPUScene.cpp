#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUScene::CGPUScene(void)
{

}

CGPUScene::~CGPUScene(void)
{

}

void CGPUScene::AddInstance(uint32_t name)
{
	if (m_nameIndex.find(name) == m_nameIndex.end()) {
		TransferData transfer;
		{
			if (m_freeIndex.empty()) {
				transfer.index = m_nameIndex.size();
			}
			else {
				transfer.index = m_freeIndex.front(); m_freeIndex.pop_front();
			}

			m_nameIndex[name] = transfer.index;
		}
		m_transferBuffer[Engine()->GetFrameCount() % 2].emplace_back(transfer);
	}
}

void CGPUScene::RemoveInstance(uint32_t name)
{
	if (m_nameIndex.find(name) != m_nameIndex.end()) {
		m_freeIndex.push_back(m_nameIndex[name]);
		m_nameIndex.erase(name);
	}
}

void CGPUScene::ModifyInstanceData(uint32_t name, const InstanceData &data)
{
	if (m_nameIndex.find(name) != m_nameIndex.end()) {
		TransferData transfer;
		{
			transfer.index = m_nameIndex[name];
			transfer.data = data;
		}
		m_transferBuffer[Engine()->GetFrameCount() % 2].emplace_back(transfer);
	}
}

uint32_t CGPUScene::GetIndex(uint32_t name) const
{
	const auto& itName = m_nameIndex.find(name);

	if (itName != m_nameIndex.end()) {
		return itName->second;
	}
	else {
		return INVALID_VALUE;
	}
}

const CGPUScene::InstanceData& CGPUScene::GetInstanceData(uint32_t index) const
{
	static InstanceData invalidInstanceData;

	if (index >= 0 && index < m_instanceBuffer.size()) {
		return m_instanceBuffer[index];
	}
	else {
		return invalidInstanceData;
	}
}

void CGPUScene::Update(void)
{
	for (int index = 0; index < m_transferBuffer[Engine()->GetFrameCount() % 2].size(); index++) {
		const TransferData& data = m_transferBuffer[Engine()->GetFrameCount() % 2][index];
		m_instanceBuffer.resize(std::max(m_instanceBuffer.size(), (size_t)data.index + 1));
		m_instanceBuffer[data.index] = data.data;
	}

	m_transferBuffer[Engine()->GetFrameCount() % 2].clear();
}
