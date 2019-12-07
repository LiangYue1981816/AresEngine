#include "GfxHeader.h"


CGfxGPUScene::CGfxGPUScene(void)
{

}

CGfxGPUScene::~CGfxGPUScene(void)
{

}

void CGfxGPUScene::AddInstance(uint32_t name)
{
	if (m_nameIndex.find(name) == m_nameIndex.end()) {
		TransferData transfer;
		{
			if (m_freeIndex.empty()) {
				transfer.index = m_instanceBuffer.size();
			}
			else {
				transfer.index = m_freeIndex.front(); m_freeIndex.pop_front();
			}
		}
		m_transferBuffer[0].emplace_back(transfer);
	}
}

void CGfxGPUScene::RemoveInstance(uint32_t name)
{
	if (m_nameIndex.find(name) != m_nameIndex.end()) {
		m_freeIndex.push_back(m_nameIndex[name]);
		m_nameIndex.erase(name);
	}
}

void CGfxGPUScene::ModifyInstance(uint32_t name, const InstanceData &data)
{
	if (m_nameIndex.find(name) != m_nameIndex.end()) {
		TransferData transfer;
		{
			transfer.index = m_nameIndex[name];
			transfer.data = data;
		}
		m_transferBuffer[0].emplace_back(transfer);
	}
}

void CGfxGPUScene::Update(void)
{
	m_transferBuffer[0].clear();
}
