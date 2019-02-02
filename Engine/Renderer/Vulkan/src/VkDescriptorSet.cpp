#include "VKRenderer.h"


CVKDescriptorSet::CVKDescriptorSet(CVKDevice *pDevice, CVKDescriptorSetLayout *pDescriptorSetLayout, VkDescriptorSet vkDescriptorSet)
	: m_pDevice(pDevice)
	, m_pDescriptorSetLayout(pDescriptorSetLayout)
	, m_vkDescriptorSet(vkDescriptorSet)
{

}

CVKDescriptorSet::~CVKDescriptorSet(void)
{

}

VkDescriptorSet CVKDescriptorSet::GetDescriptorSet(void) const
{
	return m_vkDescriptorSet;
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		m_imageInfos[name].ptrTexture2D = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer)
{
	if (m_pDescriptorSetLayout->IsUniformBlockValid(name) && m_bufferInfos.find(name) == m_bufferInfos.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::Update(void)
{
	/*
	eastl::vector<VkWriteDescriptorSet> writes;

	for (const auto &itTexture : m_ptrTexture2Ds) {
		VkWriteDescriptorSet write = {};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = m_vkDescriptorSet;
		write.dstBinding = m_pDescriptorSetLayout->GetTextureBinding(itTexture.second).binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.pImageInfo;
		write.pBufferInfo = nullptr;
		write.pTexelBufferView = nullptr;
	}
	*/
	return true;
}
