#include "VKRenderer.h"


CVKDescriptorSet::CVKDescriptorSet(CVKDevice *pDevice, CVKDescriptorPool *pDescriptorPool, CVKDescriptorLayout *pDescriptorLayout, VkDescriptorSet vkDescriptorSet)
	: m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)
	, m_pDescriptorLayout(pDescriptorLayout)
	, m_vkDescriptorSet(vkDescriptorSet)
{

}

CVKDescriptorSet::~CVKDescriptorSet(void)
{

}

uint32_t CVKDescriptorSet::GetSetIndex(void) const
{
	return m_pDescriptorLayout->GetSetIndex();
}

VkDescriptorSet CVKDescriptorSet::GetDescriptorSet(void) const
{
	return m_vkDescriptorSet;
}

CVKDescriptorPool* CVKDescriptorSet::GetDescriptorPool(void) const
{
	return m_pDescriptorPool;
}

CVKDescriptorLayout* CVKDescriptorSet::GetDescriptorLayout(void) const
{
	return m_pDescriptorLayout;
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		m_imageDescriptors[name].ptrTexture2D = ptrTexture;
		m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
		m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		m_imageDescriptors[name].ptrTexture2DArray = ptrTexture;
		m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
		m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		m_imageDescriptors[name].ptrTextureCubeMap = ptrTexture;
		m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
		m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		m_imageDescriptors[name].ptrRenderTexture = ptrRenderTexture;
		m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
		m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrRenderTexture->GetTexture();
		m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer)
{
	if (m_pDescriptorLayout->IsUniformBlockValid(name) && m_bufferDescriptors.find(name) == m_bufferDescriptors.end()) {
		m_bufferDescriptors[name].ptrUniformBuffer = ptrUniformBuffer;
		m_bufferDescriptors[name].bufferInfo.range = ptrUniformBuffer->GetSize();
		m_bufferDescriptors[name].bufferInfo.buffer = (VkBuffer)ptrUniformBuffer->GetBuffer();
		return true;
	}
	else {
		return false;
	}
}

void CVKDescriptorSet::Update(void)
{
	eastl::vector<VkWriteDescriptorSet> writes;

	for (const auto &itImage : m_imageDescriptors) {
		VkWriteDescriptorSet write = {};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = m_vkDescriptorSet;
		write.dstBinding = m_pDescriptorLayout->GetTextureBinding(itImage.first).binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = itImage.second.ptrRenderTexture.IsValid() ? VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.pImageInfo = &itImage.second.imageInfo;
		write.pBufferInfo = nullptr;
		write.pTexelBufferView = nullptr;
		writes.emplace_back(write);
	}

	for (const auto &itBuffer : m_bufferDescriptors) {
		VkWriteDescriptorSet write = {};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = m_vkDescriptorSet;
		write.dstBinding = m_pDescriptorLayout->GetUniformBlockBinding(itBuffer.first).binding;
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
