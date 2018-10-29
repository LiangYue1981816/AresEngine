#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxMeshManager.h"


CGfxMeshManager::CGfxMeshManager(void)
{

}

CGfxMeshManager::~CGfxMeshManager(void)
{
	for (const auto &itMesh : m_pMeshs) {
		delete itMesh.second;
	}

	m_pMeshs.clear();
}

CGfxMesh* CGfxMeshManager::LoadMesh(const char *szFileName, uint32_t instanceFormat)
{
	char szName[_MAX_STRING];
	sprintf(szName, "%8.8X_%8.8X", HashValue(szFileName), instanceFormat);

	uint32_t name = HashValue(szName);

	if (m_pMeshs[name] == nullptr) {
		m_pMeshs[name] = new CGfxMesh(name);
		m_pMeshs[name]->Load(szFileName, instanceFormat);
	}

	return m_pMeshs[name];
}

CGfxMesh* CGfxMeshManager::CreateMesh(uint32_t name)
{
	if (m_pMeshs[name] == nullptr) {
		m_pMeshs[name] = new CGfxMesh(name);
	}

	return m_pMeshs[name];
}

void CGfxMeshManager::DestroyMesh(CGfxMesh *pMesh)
{
	if (pMesh) {
		m_pMeshs.erase(pMesh->GetName());
		delete pMesh;
	}
}
