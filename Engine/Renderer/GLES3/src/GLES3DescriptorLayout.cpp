#include "GLES3Renderer.h"


CGLES3DescriptorLayout::CGLES3DescriptorLayout(CGLES3DescriptorLayoutManager* pManager, uint32_t set)
	: CGfxDescriptorLayout(set)
	, m_pManager(pManager)
	, m_set(set)
{

}

CGLES3DescriptorLayout::~CGLES3DescriptorLayout(void)
{
	Destroy(true);
}

void CGLES3DescriptorLayout::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3DescriptorLayout::Create(void)
{
	return true;
}

void CGLES3DescriptorLayout::Destroy(bool bClear)
{
	if (bClear) {
		m_uniformBlockBindings.clear();
		m_sampledImageBindings.clear();
		m_inputAttachmentBindings.clear();
	}
}

bool CGLES3DescriptorLayout::SetUniformBlockBinding(uint32_t name, uint32_t binding)
{
	m_uniformBlockBindings[name] = binding;
	return true;
}

bool CGLES3DescriptorLayout::SetSampledImageBinding(uint32_t name, uint32_t binding)
{
	m_sampledImageBindings[name] = binding;
	return true;
}

bool CGLES3DescriptorLayout::SetInputAttachmentBinding(uint32_t name, uint32_t binding)
{
	m_inputAttachmentBindings[name] = binding;
	return true;
}

uint32_t CGLES3DescriptorLayout::GetSetIndex(void) const
{
	return m_set;
}

uint32_t CGLES3DescriptorLayout::GetUniformBlockBinding(uint32_t name) const
{
	const auto& itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		return itBinding->second;
	}
	else {
		return -1;
	}
}

uint32_t CGLES3DescriptorLayout::GetSampledImageBinding(uint32_t name) const
{
	const auto& itBinding = m_sampledImageBindings.find(name);

	if (itBinding != m_sampledImageBindings.end()) {
		return itBinding->second;
	}
	else {
		return -1;
	}
}

uint32_t CGLES3DescriptorLayout::GetInputAttachmentBinding(uint32_t name) const
{
	const auto& itBinding = m_inputAttachmentBindings.find(name);

	if (itBinding != m_inputAttachmentBindings.end()) {
		return itBinding->second;
	}
	else {
		return -1;
	}
}

bool CGLES3DescriptorLayout::IsUniformBlockValid(uint32_t name) const
{
	return GetUniformBlockBinding(name) != -1;
}

bool CGLES3DescriptorLayout::IsSampledImageValid(uint32_t name) const
{
	return GetSampledImageBinding(name) != -1;
}

bool CGLES3DescriptorLayout::IsInputAttachmentValid(uint32_t name) const
{
	return GetInputAttachmentBinding(name) != -1;
}

bool CGLES3DescriptorLayout::IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const
{
	ASSERT(ptrLayout);

	if (m_set != ((CGLES3DescriptorLayout*)ptrLayout.GetPointer())->m_set) {
		return false;
	}

	if (m_uniformBlockBindings != ((CGLES3DescriptorLayout*)ptrLayout.GetPointer())->m_uniformBlockBindings) {
		return false;
	}

	if (m_sampledImageBindings != ((CGLES3DescriptorLayout*)ptrLayout.GetPointer())->m_sampledImageBindings) {
		return false;
	}

	if (m_inputAttachmentBindings != ((CGLES3DescriptorLayout*)ptrLayout.GetPointer())->m_inputAttachmentBindings) {
		return false;
	}

	return true;
}
