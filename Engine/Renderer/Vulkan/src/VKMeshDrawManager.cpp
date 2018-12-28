#include "VKRenderer.h"
#include "ResourceLoader.h"


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

CVKDevice* CVKMeshDrawManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKMeshDrawManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pMeshDraws.find(name) != m_pMeshDraws.end();
	}
}

CGfxMeshDraw* CVKMeshDrawManager::Create(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshDraws[name] == nullptr) {
			m_pMeshDraws[name] = new CVKMeshDraw(this, name, ptrMesh, indexDraw, instanceFormat, instanceBinding);
		}

		return (CVKMeshDraw *)m_pMeshDraws[name];
	}
}

void CVKMeshDrawManager::Destroy(CGfxMeshDraw *pMeshDraw)
{
	mutex_autolock autolock(&lock);
	{
		if (pMeshDraw) {
			m_pMeshDraws.erase(pMeshDraw->GetName());
			delete pMeshDraw;
		}
	}
}
