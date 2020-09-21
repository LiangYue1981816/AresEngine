#include "VKRenderer.h"


CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	: CGfxDescriptorSet(name, ptrDescriptorLayout)
	, m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)

	, m_vkDescriptorSet(VK_NULL_HANDLE)
	, m_ptrDescriptorLayout(ptrDescriptorLayout)
{
	VkDescriptorSetLayout vkDescriptorSetLayout = ((CVKDescriptorLayout*)ptrDescriptorLayout.GetPointer())->GetDescriptorLayout();
	VkDescriptorSetAllocateInfo descriptorAllocInfo = {};
	descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorAllocInfo.pNext = nullptr;
	descriptorAllocInfo.descriptorPool = pDescriptorPool->GetDescriptorPool();
	descriptorAllocInfo.descriptorSetCount = 1;
	descriptorAllocInfo.pSetLayouts = &vkDescriptorSetLayout;
	CALL_VK_FUNCTION_ASSERT(vkAllocateDescriptorSets(m_pDevice->GetDevice(), &descriptorAllocInfo, &m_vkDescriptorSet));
}

CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
	: CGfxDescriptorSet(name, ptrDescriptorSetCopyFrom->GetDescriptorLayout())
	, m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)

	, m_vkDescriptorSet(VK_NULL_HANDLE)
	, m_ptrDescriptorLayout(ptrDescriptorSetCopyFrom->GetDescriptorLayout())
{
	VkDescriptorSetLayout vkDescriptorSetLayout = ((CVKDescriptorLayout*)ptrDescriptorSetCopyFrom->GetDescriptorLayout().GetPointer())->GetDescriptorLayout();
	VkDescriptorSetAllocateInfo descriptorAllocInfo = {};
	descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorAllocInfo.pNext = nullptr;
	descriptorAllocInfo.descriptorPool = pDescriptorPool->GetDescriptorPool();
	descriptorAllocInfo.descriptorSetCount = 1;
	descriptorAllocInfo.pSetLayouts = &vkDescriptorSetLayout;
	CALL_VK_FUNCTION_ASSERT(vkAllocateDescriptorSets(m_pDevice->GetDevice(), &descriptorAllocInfo, &m_vkDescriptorSet));

	m_imageDescriptorInfos = ((CVKDescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_imageDescriptorInfos;
	m_bufferDescriptorInfos = ((CVKDescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_bufferDescriptorInfos;
}

CVKDescriptorSet::~CVKDescriptorSet(void)
{

}

void CVKDescriptorSet::Release(void)
{
	m_pDescriptorPool->GetDescriptorSetManager()->Destroy(this);
}

CVKDescriptorPool* CVKDescriptorSet::GetDescriptorPool(void) const
{
	return m_pDescriptorPool;
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrBuffer, uint32_t offset, uint32_t range)
{
	if (m_ptrDescriptorLayout->IsUniformBlockValid(name) && ptrBuffer->GetSize() >= offset + range) {
		m_bufferDescriptorInfos[name].SetUniformBuffer(m_ptrDescriptorLayout->GetUniformBlockBinding(name), ptrBuffer, offset, range);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrBuffer, uint32_t offset, uint32_t range)
{
	if (m_ptrDescriptorLayout->IsStorageBlockValid(name) && ptrBuffer->GetSize() >= offset + range) {
		m_bufferDescriptorInfos[name].SetStorageBuffer(m_ptrDescriptorLayout->GetStorageBlockBinding(name), ptrBuffer, offset, range);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImage2D(uint32_t name, const CGfxTexture2DPtr ptrImage, uint32_t level)
{
	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImage2D(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage, level);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImage2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrImage, uint32_t level, uint32_t layer)
{
	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImage2DArray(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage, level, layer);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImageCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrImage, uint32_t level, uint32_t layer)
{
	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImageCubemap(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage, level, layer);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImageRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrImage)
{
	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImageRenderTexture(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].SetTexture2D(m_ptrDescriptorLayout->GetSampledImageBinding(name), ptrTexture, (CGfxSampler*)pSampler);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].SetTexture2DArray(m_ptrDescriptorLayout->GetSampledImageBinding(name), ptrTexture, (CGfxSampler*)pSampler);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].SetTextureCubemap(m_ptrDescriptorLayout->GetSampledImageBinding(name), ptrTexture, (CGfxSampler*)pSampler);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].SetRenderTexture(m_ptrDescriptorLayout->GetSampledImageBinding(name), ptrTexture, (CGfxSampler*)pSampler);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsInputAttachmentValid(name)) {
		m_imageDescriptorInfos[name].SetInputAttachmentTexture(m_ptrDescriptorLayout->GetInputAttachmentBinding(name), ptrTexture, (CGfxSampler*)pSampler);
		return true;
	}
	else {
		return false;
	}
}

const CGfxDescriptorLayoutPtr CVKDescriptorSet::GetDescriptorLayout(void) const
{
	return m_ptrDescriptorLayout;
}

const DescriptorImageInfo* CVKDescriptorSet::GetDescriptorImageInfo(uint32_t name) const
{
	const auto& itDescriptorInfo = m_imageDescriptorInfos.find(name);

	if (itDescriptorInfo != m_imageDescriptorInfos.end()) {
		return &itDescriptorInfo->second;
	}
	else {
		return nullptr;
	}
}

const DescriptorBufferInfo* CVKDescriptorSet::GetDescriptorBufferInfo(uint32_t name) const
{
	const auto& itDescriptorInfo = m_bufferDescriptorInfos.find(name);

	if (itDescriptorInfo != m_bufferDescriptorInfos.end()) {
		return &itDescriptorInfo->second;
	}
	else {
		return nullptr;
	}
}
