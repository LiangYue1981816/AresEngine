#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKMeshManager::CVKMeshManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKMeshManager::~CVKMeshManager(void)
{
	for (const auto &itMesh : m_pMeshs) {
		delete itMesh.second;
	}

	m_pMeshs.clear();
}

CVKDevice* CVKMeshManager::GetDevice(void) const
{
	return m_pDevice;
}

bool CVKMeshManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pMeshs.find(name) != m_pMeshs.end();
	}
}

CVKMesh* CVKMeshManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CVKMesh(this, name);
		}

		return (CVKMesh *)m_pMeshs[name];
	}
}

CVKMesh* CVKMeshManager::Create(const char *szFileName, uint32_t vertexBinding)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CVKMesh(this, name);
			ResourceLoader()->LoadMesh(szFileName, m_pMeshs[name], vertexBinding);
		}

		return (CVKMesh *)m_pMeshs[name];
	}
}

void CVKMeshManager::Destroy(CGfxMesh *pMesh)
{
	mutex_autolock autolock(&lock);
	{
		if (pMesh) {
			m_pMeshs.erase(pMesh->GetName());
			delete pMesh;
		}
	}
}
