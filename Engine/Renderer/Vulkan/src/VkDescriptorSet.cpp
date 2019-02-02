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

CVKDescriptorSetLayout* CVKDescriptorSet::GetDescriptorSetLayout(void) const
{
	return m_pDescriptorSetLayout;
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr &ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		m_imageInfos[name].ptrTexture2D = ptrTexture;
		m_imageInfos[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageInfos[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
		m_imageInfos[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr &ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		m_imageInfos[name].ptrTexture2DArray = ptrTexture;
		m_imageInfos[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageInfos[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
		m_imageInfos[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr &ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		m_imageInfos[name].ptrTextureCubeMap = ptrTexture;
		m_imageInfos[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageInfos[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
		m_imageInfos[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr &ptrRenderTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorSetLayout->IsTextureValid(name) && m_imageInfos.find(name) == m_imageInfos.end()) {
		m_imageInfos[name].ptrRenderTexture = ptrRenderTexture;
		m_imageInfos[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageInfos[name].imageInfo.imageView = (VkImageView)ptrRenderTexture->GetTexture();
		m_imageInfos[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr &ptrUniformBuffer)
{
	if (m_pDescriptorSetLayout->IsUniformBlockValid(name) && m_bufferInfos.find(name) == m_bufferInfos.end()) {
		m_bufferInfos[name].ptrUniformBuffer = ptrUniformBuffer;
		m_bufferInfos[name].bufferInfo.range = ptrUniformBuffer->GetSize();
		m_bufferInfos[name].bufferInfo.buffer = (VkBuffer)ptrUniformBuffer->GetBuffer();
		return true;
	}
	else {
		return false;
	}
}

void CVKDescriptorSet::Update(void)
{
	eastl::vector<VkWriteDescriptorSet> writes;

	for (const auto &itImage : m_imageInfos) {
		VkWriteDescriptorSet write = {};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = m_vkDescriptorSet;
		write.dstBinding = m_pDescriptorSetLayout->GetTextureBinding(itImage.first).binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = itImage.second.ptrRenderTexture.IsValid() ? VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.pImageInfo = &itImage.second.imageInfo;
		write.pBufferInfo = nullptr;
		write.pTexelBufferView = nullptr;
		writes.emplace_back(write);
	}

	for (const auto &itBuffer : m_bufferInfos) {
		VkWriteDescriptorSet write = {};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = m_vkDescriptorSet;
		write.dstBinding = m_pDescriptorSetLayout->GetUniformBlockBinding(itBuffer.first).binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		write.pImageInfo = nullptr;
		write.pBufferInfo = &itBuffer.second.bufferInfo;
		write.pTexelBufferView = nullptr;
		writes.emplace_back(write);
	}

	vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, nullptr);
}
