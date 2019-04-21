#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKMeshManager::CVKMeshManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKMeshManager::~CVKMeshManager(void)
{
	for (const auto& itMesh : m_pMeshs) {
		delete itMesh.second;
	}
}

CVKMesh* CVKMeshManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itMesh = m_pMeshs.find(name);

		if (itMesh != m_pMeshs.end()) {
			return itMesh->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKMesh* CVKMeshManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CVKMesh(m_pDevice, this, name);
		}

		return m_pMeshs[name];
	}
}

CVKMesh* CVKMeshManager::Create(const char* szFileName, uint32_t vertexBinding)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CVKMesh(m_pDevice, this, name);
			ResourceLoader()->LoadMesh(szFileName, m_pMeshs[name], vertexBinding);
		}

		return m_pMeshs[name];
	}
}

void CVKMeshManager::Destroy(CVKMesh* pMesh)
{
	mutex_autolock autolock(&lock);
	{
		if (pMesh) {
			m_pMeshs.erase(pMesh->GetName());
			delete pMesh;
		}
	}
}
