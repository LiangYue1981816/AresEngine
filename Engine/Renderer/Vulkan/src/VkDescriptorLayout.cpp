#include "VKRenderer.h"


CVKDescriptorLayout::CVKDescriptorLayout(CVKDevice* pDevice, uint32_t set)
	: CGfxDescriptorLayout(set)
	, m_pDevice(pDevice)
	, m_set(set)
{

}

CVKDescriptorLayout::~CVKDescriptorLayout(void)
{

}

void CVKDescriptorLayout::Release(void)
{

}

bool CVKDescriptorLayout::Create(void)
{
	return true;
}

void CVKDescriptorLayout::Destroy(void)
{

}

bool CVKDescriptorLayout::SetUniformBlockBinding(uint32_t name, uint32_t binding)
{
	return true;
}

bool CVKDescriptorLayout::SetSampledImageBinding(uint32_t name, uint32_t binding)
{
	return true;
}

uint32_t CVKDescriptorLayout::GetSetIndex(void) const
{
	return m_set;
}

uint32_t CVKDescriptorLayout::GetUniformBlockBinding(uint32_t name) const
{
	return 0;
}

uint32_t CVKDescriptorLayout::GetSampledImageBinding(uint32_t name) const
{
	return 0;
}

bool CVKDescriptorLayout::IsUniformBlockValid(uint32_t name) const
{
	return true;
}

bool CVKDescriptorLayout::IsSampledImageValid(uint32_t name) const
{
	return true;
}

bool CVKDescriptorLayout::IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const
{
	return true;
}
