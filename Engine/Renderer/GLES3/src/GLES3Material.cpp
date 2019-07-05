#include "GLES3Renderer.h"


CGLES3Material::CGLES3Material(CGLES3MaterialManager* pManager, uint32_t name)
	: CGfxMaterial(name)
	, m_pManager(pManager)
{

}

CGLES3Material::CGLES3Material(CGLES3MaterialManager* pManager, uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom)
	: CGfxMaterial(name)
	, m_pManager(pManager)
{
	for (const auto& itPass : ((CGLES3Material *)ptrMaterialCopyFrom.GetPointer())->m_pPasses) {
		m_pPasses[itPass.first] = new CGLES3MaterialPass(name, itPass.second);
	}
}

CGLES3Material::~CGLES3Material(void)
{
	Destroy();
}

void CGLES3Material::Release(void)
{
	m_pManager->Destroy(this);
}

CGfxMaterialPass* CGLES3Material::GetPass(uint32_t name)
{
	const auto& itPass = m_pPasses.find(name);

	if (itPass != m_pPasses.end()) {
		return itPass->second;
	}
	else {
		return nullptr;
	}
}

CGfxMaterialPass* CGLES3Material::CreatePass(uint32_t name)
{
	if (m_pPasses[name] == nullptr) {
		m_pPasses[name] = new CGLES3MaterialPass(name);
	}

	return m_pPasses[name];
}

void CGLES3Material::Destroy(void)
{
	for (const auto& itPass : m_pPasses) {
		delete itPass.second;
	}

	m_pPasses.clear();
}
