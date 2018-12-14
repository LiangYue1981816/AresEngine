#include "GLES3Renderer.h"
#include "ResourceLoader.h"


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

bool CGLES3MeshManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pMeshs.find(name) != m_pMeshs.end();
	}
}

CGLES3Mesh* CGLES3MeshManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CGLES3Mesh(this, name);
		}

		return (CGLES3Mesh *)m_pMeshs[name];
	}
}

CGLES3Mesh* CGLES3MeshManager::Create(const char *szFileName, uint32_t instanceFormat)
{
	char szName[_MAX_STRING];
	sprintf(szName, "%8.8X_%8.8X", HashValue(szFileName), instanceFormat);

	uint32_t name = HashValue(szName);

	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CGLES3Mesh(this, name);
			ResourceLoader()->LoadMesh(szFileName, m_pMeshs[name], instanceFormat);
		}

		return (CGLES3Mesh *)m_pMeshs[name];
	}
}

void CGLES3MeshManager::Destroy(CGfxMesh *pMesh)
{
	mutex_autolock autolock(&lock);
	{
		if (pMesh) {
			m_pMeshs.erase(pMesh->GetName());
			delete pMesh;
		}
	}
}
