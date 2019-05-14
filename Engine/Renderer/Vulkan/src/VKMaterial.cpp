#include "VKRenderer.h"


CVKMaterial::CVKMaterial(CVKDevice* pDevice, CVKMaterialManager* pManager, uint32_t name)
	: CGfxMaterial(name)
	, m_name(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
{

}

CVKMaterial::~CVKMaterial(void)
{
	Destroy();
}

void CVKMaterial::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CVKMaterial::GetName(void) const
{
	return m_name;
}

CGfxMaterialPass* CVKMaterial::GetPass(uint32_t name)
{
	const auto& itPass = m_pPasses.find(name);

	if (itPass != m_pPasses.end()) {
		return itPass->second;
	}
	else {
		return nullptr;
	}
}

CGfxMaterialPass* CVKMaterial::CreatePass(uint32_t name)
{
	if (m_pPasses[name] == nullptr) {
		m_pPasses[name] = new CVKMaterialPass(m_pDevice, name);
	}

	return m_pPasses[name];
}

void CVKMaterial::Destroy(void)
{
	for (const auto& itPass : m_pPasses) {
		delete itPass.second;
	}

	m_pPasses.clear();
}
