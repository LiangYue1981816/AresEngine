#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3MeshManager::CGLES3MeshManager(void)
{

}

CGLES3MeshManager::~CGLES3MeshManager(void)
{
	for (const auto& itMesh : m_pMeshs) {
		delete itMesh.second;
	}
}

CGLES3Mesh* CGLES3MeshManager::Get(uint32_t name)
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

CGLES3Mesh* CGLES3MeshManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CGLES3Mesh(this, name);
		}

		return m_pMeshs[name];
	}
}

CGLES3Mesh* CGLES3MeshManager::Create(const char* szFileName, int vertexBinding)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pMeshs[name] == nullptr) {
			m_pMeshs[name] = new CGLES3Mesh(this, name);
			ResourceLoader()->LoadMesh(szFileName, m_pMeshs[name], vertexBinding);
		}

		return m_pMeshs[name];
	}
}

void CGLES3MeshManager::Destroy(CGLES3Mesh* pMesh)
{
	ASSERT(pMesh);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pMeshs.find(pMesh->GetName()) != m_pMeshs.end()) {
				m_pMeshs.erase(pMesh->GetName());
			}
		}
	}
	delete pMesh;
}
