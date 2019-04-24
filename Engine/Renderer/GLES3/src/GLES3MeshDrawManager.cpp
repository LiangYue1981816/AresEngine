#include "GLES3Renderer.h"


CGLES3MeshDrawManager::CGLES3MeshDrawManager(void)
{

}

CGLES3MeshDrawManager::~CGLES3MeshDrawManager(void)
{
	for (const auto& itMeshDraw : m_pMeshDraws) {
		delete itMeshDraw.second;
	}
}

CGLES3MeshDraw* CGLES3MeshDrawManager::Get(uint32_t name)
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

CGLES3MeshDraw* CGLES3MeshDrawManager::Create(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMeshDraws[name] == nullptr) {
			m_pMeshDraws[name] = new CGLES3MeshDraw(this, name, ptrMesh, nameDraw, instanceFormat, instanceBinding);
		}

		return m_pMeshDraws[name];
	}
}

void CGLES3MeshDrawManager::Destroy(CGLES3MeshDraw* pMeshDraw)
{
	mutex_autolock autolock(&lock);
	{
		if (pMeshDraw) {
			if (m_pMeshDraws.find(pMeshDraw->GetName()) != m_pMeshDraws.end()) {
				m_pMeshDraws.erase(pMeshDraw->GetName());
				delete pMeshDraw;
			}
		}
	}
}
