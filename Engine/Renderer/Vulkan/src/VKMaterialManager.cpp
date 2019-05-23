#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKMaterialManager::CVKMaterialManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKMaterialManager::~CVKMaterialManager(void)
{
	for (const auto& itMaterial : m_pMaterials) {
		delete itMaterial.second;
	}
}

CVKMaterial* CVKMaterialManager::Get(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		const auto& itMaterial = m_pMaterials.find(name);

		if (itMaterial != m_pMaterials.end()) {
			return itMaterial->second;
		}
		else {
			return nullptr;
		}
	}
}

CVKMaterial* CVKMaterialManager::Create(uint32_t name)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMaterials[name] == nullptr) {
			m_pMaterials[name] = new CVKMaterial(m_pDevice, this, name);
		}

		return m_pMaterials[name];
	}
}

CVKMaterial* CVKMaterialManager::Create(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel, int numLevels)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValue(szFileName);

		if (m_pMaterials[name] == nullptr) {
			m_pMaterials[name] = new CVKMaterial(m_pDevice, this, name);
			ResourceLoader()->LoadMaterial(szFileName, m_pMaterials[name], vertexBinding, instanceBinding, baseLevel, numLevels);
		}

		return m_pMaterials[name];
	}
}

void CVKMaterialManager::Destroy(CVKMaterial* pMaterial)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pMaterial);

		if (m_pMaterials.find(pMaterial->GetName()) != m_pMaterials.end()) {
			m_pMaterials.erase(pMaterial->GetName());
			delete pMaterial;
		}
	}
}
