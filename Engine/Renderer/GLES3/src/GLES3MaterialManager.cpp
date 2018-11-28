#include "GfxHeader.h"


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

CGfxMaterial* CGLES3MaterialManager::Create(const char *szFileName)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pMaterials[name] == nullptr) {
			m_pMaterials[name] = new CGLES3Material(this, name);
		}

		return m_pMaterials[name];
	}
}

void CGLES3MaterialManager::Destroy(CGfxMaterial *pMaterial)
{
	mutex_autolock autolock(&lock);
	{
		if (pMaterial) {
			m_pMaterials.erase(pMaterial->GetName());
			delete pMaterial;
		}
	}
}
