#include "GLES3Renderer.h"
#include "ResourceLoader.h"


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

CGfxMeshDraw* CGLES3MeshDrawManager::Create(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceBinding, uint32_t instanceFormat)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3MeshDraw *pMeshDraw = new CGLES3MeshDraw(this);
		m_pMeshDraws[pMeshDraw] = pMeshDraw;
		return pMeshDraw;
	}
}

void CGLES3MeshDrawManager::Destroy(CGfxMeshDraw *pMeshDraw)
{
	mutex_autolock autolock(&lock);
	{
		if (pMeshDraw) {
			m_pMeshDraws.erase(pMeshDraw);
			delete pMeshDraw;
		}
	}
}
