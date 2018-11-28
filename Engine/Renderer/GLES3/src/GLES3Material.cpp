#include "GfxHeader.h"


CGLES3Material::CGLES3Material(CGLES3MaterialManager *pManager, uint32_t name)
	: CGfxMaterial(name)
	, m_pManager(pManager)
{

}

CGLES3Material::~CGLES3Material(void)
{
	Destroy();
}

void CGLES3Material::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3Material::CreatePass(uint32_t name)
{
	if (m_pPasses[name] == nullptr) {
		m_pPasses[name] = new CGLES3MaterialPass(name);
		return true;
	}

	return false;
}

void CGLES3Material::Destroy(void)
{
	for (const auto &itPass : m_pPasses) {
		delete itPass.second;
	}

	m_pPasses.clear();
}

CGfxMaterialPass* CGLES3Material::GetPass(uint32_t name)
{
	const auto &itPass = m_pPasses.find(name);
	return itPass != m_pPasses.end() ? itPass->second : nullptr;
}
