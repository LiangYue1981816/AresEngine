#include "VKRenderer.h"


CVKMeshDrawManager::CVKMeshDrawManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKMeshDrawManager::~CVKMeshDrawManager(void)
{
	for (const auto& itMeshDraw : m_pMeshDraws) {
		delete itMeshDraw.second;
	}
}

CVKMeshDraw* CVKMeshDrawManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itMeshDraw = m_pMeshDraws.find(name);

		if (itMeshDraw != m_pMeshDraws.end()) {
			return itMeshDraw->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKMeshDraw* CVKMeshDrawManager::Create(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshDraws[name] == nullptr) {
			m_pMeshDraws[name] = new CVKMeshDraw(m_pDevice, this, name, ptrMesh, nameDraw);
		}

		return m_pMeshDraws[name];
	}
}

void CVKMeshDrawManager::Destroy(CVKMeshDraw* pMeshDraw)
{
	ASSERT(pMeshDraw);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pMeshDraws.find(pMeshDraw->GetName()) != m_pMeshDraws.end()) {
				m_pMeshDraws.erase(pMeshDraw->GetName());
			}
		}
	}
	delete pMeshDraw;
}
