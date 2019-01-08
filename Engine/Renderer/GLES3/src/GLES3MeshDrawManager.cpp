#include "GLES3Renderer.h"


CGLES3MeshDrawManager::CGLES3MeshDrawManager(void)
{

}

CGLES3MeshDrawManager::~CGLES3MeshDrawManager(void)
{
	for (const auto &itMeshDraw : m_pMeshDraws) {
		delete itMeshDraw.second;
	}

	m_pMeshDraws.clear();
}

bool CGLES3MeshDrawManager::IsHave(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		return m_pMeshDraws.find(name) != m_pMeshDraws.end();
	}
}

CGfxMeshDraw* CGLES3MeshDrawManager::Create(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshDraws[name] == nullptr) {
			m_pMeshDraws[name] = new CGLES3MeshDraw(this, name, ptrMesh, indexDraw, instanceFormat, instanceBinding);
		}

		return (CGLES3MeshDraw *)m_pMeshDraws[name];
	}
}

void CGLES3MeshDrawManager::Destroy(CGfxMeshDraw *pMeshDraw)
{
	mutex_autolock autolock(&lock);
	{
		if (pMeshDraw) {
			m_pMeshDraws.erase(pMeshDraw->GetName());
			delete pMeshDraw;
		}
	}
}
