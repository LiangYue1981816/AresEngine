#include "GLES3Renderer.h"


CGLES3DescriptorLayout::CGLES3DescriptorLayout(CGLES3DescriptorLayoutManager *pManager, uint32_t set)
	: CGfxDescriptorLayout(set)
	, m_pManager(pManager)
	, m_set(set)
{

}

CGLES3DescriptorLayout::~CGLES3DescriptorLayout(void)
{
	Destroy();
}

void CGLES3DescriptorLayout::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3DescriptorLayout::GetDescriptorLayout(void) const
{
	return nullptr;
}

uint32_t CGLES3DescriptorLayout::GetSetIndex(void) const
{
	return m_set;
}

bool CGLES3DescriptorLayout::Create(void)
{
	return true;
}

void CGLES3DescriptorLayout::Destroy(void)
{
	m_sampledImageBindings.clear();
	m_uniformBlockBindings.clear();
}

bool CGLES3DescriptorLayout::SetInputAttachmentBinding(uint32_t name, uint32_t binding)
{
	if (m_sampledImageBindings.find(name) == m_sampledImageBindings.end()) {
		m_sampledImageBindings[name] = binding;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorLayout::SetSampledImageBinding(uint32_t name, uint32_t binding)
{
	if (m_sampledImageBindings.find(name) == m_sampledImageBindings.end()) {
		m_sampledImageBindings[name] = binding;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorLayout::SetUniformBlockBinding(uint32_t name, uint32_t binding)
{
	if (m_uniformBlockBindings.find(name) == m_uniformBlockBindings.end()) {
		m_uniformBlockBindings[name] = binding;
		return true;
	}
	else {
		return false;
	}
}

uint32_t CGLES3DescriptorLayout::GetTextureBinding(uint32_t name) const
{
	const auto &itBinding = m_sampledImageBindings.find(name);

	if (itBinding != m_sampledImageBindings.end()) {
		return itBinding->second;
	}
	else {
		return -1;
	}
}

uint32_t CGLES3DescriptorLayout::GetUniformBlockBinding(uint32_t name) const
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		return itBinding->second;
	}
	else {
		return -1;
	}
}

bool CGLES3DescriptorLayout::IsTextureValid(uint32_t name) const
{
	return GetTextureBinding(name) != -1;
}

bool CGLES3DescriptorLayout::IsUniformBlockValid(uint32_t name) const
{
	return GetUniformBlockBinding(name) != -1;
}

bool CGLES3DescriptorLayout::IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const
{
	if (m_set != ((CGLES3DescriptorLayout *)ptrLayout.GetPointer())->m_set) {
		return false;
	}

	if (m_sampledImageBindings != ((CGLES3DescriptorLayout *)ptrLayout.GetPointer())->m_sampledImageBindings) {
		return false;
	}

	if (m_uniformBlockBindings != ((CGLES3DescriptorLayout *)ptrLayout.GetPointer())->m_uniformBlockBindings) {
		return false;
	}

	return true;
}
