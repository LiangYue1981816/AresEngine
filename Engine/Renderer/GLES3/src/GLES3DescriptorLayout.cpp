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

}

bool CGLES3DescriptorLayout::SetInputAttachmentBinding(uint32_t name, uint32_t binding)
{
	m_sampledImageBindings[name] = binding;
	return true;
}

bool CGLES3DescriptorLayout::SetSampledImageBinding(uint32_t name, uint32_t binding)
{
	m_sampledImageBindings[name] = binding;
	return true;
}

bool CGLES3DescriptorLayout::SetUniformBlockBinding(uint32_t name, uint32_t binding)
{
	m_uniformBlockBindings[name] = binding;
	return true;
}

bool CGLES3DescriptorLayout::SetPushConstantBinding(uint32_t name, uint32_t binding, uint32_t offset, uint32_t size)
{
	m_pushConstantBindings[name] = binding;
	return true;
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

uint32_t CGLES3DescriptorLayout::GetPushConstantBinding(uint32_t name) const
{
	const auto &itBinding = m_pushConstantBindings.find(name);

	if (itBinding != m_pushConstantBindings.end()) {
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

bool CGLES3DescriptorLayout::IsPushConstantValid(uint32_t name) const
{
	return GetPushConstantBinding(name) != -1;
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

	if (m_pushConstantBindings != ((CGLES3DescriptorLayout *)ptrLayout.GetPointer())->m_pushConstantBindings) {
		return false;
	}

	return true;
}
