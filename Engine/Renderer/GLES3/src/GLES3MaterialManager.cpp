#include "GLES3Renderer.h"
#include "ResourceLoader.h"


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

CGLES3Material* CGLES3MaterialManager::Create(const char *szFileName)
{
	uint32_t name = HashValue(szFileName);

	mutex_autolock autolock(&lock);
	{
		if (m_pMaterials[name] == nullptr) {
			m_pMaterials[name] = new CGLES3Material(this, name);
			ResourceLoader()->LoadMaterial(szFileName, m_pMaterials[name]);
		}

		return (CGLES3Material *)m_pMaterials[name];
	}
}

CGLES3Material* CGLES3MaterialManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMaterials[name] == nullptr) {
			m_pMaterials[name] = new CGLES3Material(this, name);
		}

		return (CGLES3Material *)m_pMaterials[name];
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
