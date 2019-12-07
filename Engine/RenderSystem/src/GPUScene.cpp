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

void CGPUScene::ModifyInstance(uint32_t name, const InstanceData &data)
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

void CGPUScene::Update(void)
{
	m_transferBuffer[Engine()->GetFrameCount() % 2].clear();
}
