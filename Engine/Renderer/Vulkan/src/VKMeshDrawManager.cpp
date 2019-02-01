#include "VKRenderer.h"


CVKMeshDrawManager::CVKMeshDrawManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKMeshDrawManager::~CVKMeshDrawManager(void)
{
	for (const auto &itMeshDraw : m_pMeshDraws) {
		delete itMeshDraw.second;
	}

	m_pMeshDraws.clear();
}

CVKMeshDraw* CVKMeshDrawManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto &itMeshDraw = m_pMeshDraws.find(name);

		if (itMeshDraw != m_pMeshDraws.end()) {
			return itMeshDraw->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKMeshDraw* CVKMeshDrawManager::Create(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshDraws[name] == nullptr) {
			m_pMeshDraws[name] = new CVKMeshDraw(m_pDevice, this, name, ptrMesh, indexDraw, instanceFormat, instanceBinding);
		}

		return m_pMeshDraws[name];
	}
}

void CVKMeshDrawManager::Destroy(CVKMeshDraw *pMeshDraw)
{
	mutex_autolock autolock(&lock);
	{
		if (pMeshDraw) {
			m_pMeshDraws.erase(pMeshDraw->GetName());
			delete pMeshDraw;
		}
	}
}
