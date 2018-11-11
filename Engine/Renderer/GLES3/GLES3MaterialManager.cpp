#include "GLES3Renderer.h"
#include "GLES3MaterialManager.h"


CGLES3MaterialManager::CGLES3MaterialManager(void)
{

}

CGLES3MaterialManager::~CGLES3MaterialManager(void)
{
	for (const auto &itMaterial : m_pMaterials) {
		delete itMaterial.second;
	}

	m_pMaterials.clear();
}

CGLES3Material* CGLES3MaterialManager::LoadMaterial(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	if (m_pMaterials[name] == nullptr) {
		m_pMaterials[name] = new CGLES3Material(this, name);
		m_pMaterials[name]->Load(szFileName);
	}

	return m_pMaterials[name];
}

void CGLES3MaterialManager::DestroyMaterial(CGLES3Material *pMaterial)
{
	if (pMaterial) {
		m_pMaterials.erase(pMaterial->GetName());
		delete pMaterial;
	}
}
