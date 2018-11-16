#include "GLES3Renderer.h"
#include "GLES3MeshManager.h"


CGLES3MeshManager::CGLES3MeshManager(void)
{

}

CGLES3MeshManager::~CGLES3MeshManager(void)
{
	for (const auto &itMesh : m_pMeshs) {
		delete itMesh.second;
	}

	m_pMeshs.clear();
}

CGLES3Mesh* CGLES3MeshManager::LoadMesh(const char *szFileName, uint32_t instanceFormat)
{
	mutex_autolock mutex(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X_%8.8X", HashValue(szFileName), instanceFormat);

		uint32_t name = HashValue(szName);

		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CGLES3Mesh(this, name);
			m_pMeshs[name]->Load(szFileName, instanceFormat);
		}

		return m_pMeshs[name];
	}
}

CGLES3Mesh* CGLES3MeshManager::CreateMesh(uint32_t name, uint32_t instanceFormat)
{
	mutex_autolock mutex(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CGLES3Mesh(this, name, instanceFormat);
		}

		return m_pMeshs[name];
	}
}

void CGLES3MeshManager::DestroyMesh(CGLES3Mesh *pMesh)
{
	mutex_autolock mutex(&lock);
	{
		if (pMesh) {
			m_pMeshs.erase(pMesh->GetName());
			delete pMesh;
		}
	}
}
