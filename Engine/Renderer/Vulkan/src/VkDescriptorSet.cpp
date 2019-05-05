#include "VKRenderer.h"


CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	: CGfxDescriptorSet(ptrDescriptorLayout)
	, m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)
{

}

CVKDescriptorSet::~CVKDescriptorSet(void)
{

}

void CVKDescriptorSet::Release(void)
{
	m_pDescriptorPool->FreeDescriptorSet(this);
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler)
{
	return true;
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler)
{
	return true;
}

bool CVKDescriptorSet::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler)
{
	return true;
}

bool CVKDescriptorSet::SetTextureInputAttachment(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	return true;
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range)
{
	return true;
}

const CVKDescriptorPool* CVKDescriptorSet::GetDescriptorPool(void) const
{
	return m_pDescriptorPool;
}

const CGfxDescriptorLayoutPtr CVKDescriptorSet::GetDescriptorLayout(void) const
{
	return m_ptrDescriptorLayout;
}

const DescriptorImageInfo* CVKDescriptorSet::GetDescriptorImageInfo(uint32_t name) const
{
	return nullptr;
}

const DescriptorBufferInfo* CVKDescriptorSet::GetDescriptorBufferInfo(uint32_t name) const
{
	return nullptr;
}

/*
CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, CVKDescriptorLayout* pDescriptorLayout)
	: m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)
	, m_pDescriptorLayout(pDescriptorLayout)
	, m_vkDescriptorSet(VK_NULL_HANDLE)

	, m_bDirty(false)
{
	VkDescriptorSetLayout vkDescriptorLayout = m_pDescriptorLayout->GetDescriptorSetLayout();
	VkDescriptorSetAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.descriptorPool = m_pDescriptorPool->GetDescriptorPool();
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &vkDescriptorLayout;
	vkAllocateDescriptorSets(m_pDevice->GetDevice(), &allocateInfo, &m_vkDescriptorSet);
}

CVKDescriptorSet::~CVKDescriptorSet(void)
{
	if (m_vkDescriptorSet) {
		vkFreeDescriptorSets(m_pDevice->GetDevice(), m_pDescriptorPool->GetDescriptorPool(), 1, &m_vkDescriptorSet);
	}
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

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		if (m_imageDescriptors[name].ptrTexture2D != ptrTexture || m_imageDescriptors[name].pSampler != pSampler) {
			m_imageDescriptors[name].ptrTexture2D = ptrTexture;
			m_imageDescriptors[name].pSampler = (CVKSampler*)pSampler;
			m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
			m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
			m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_bDirty = true;
		}
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		if (m_imageDescriptors[name].ptrTexture2DArray != ptrTexture || m_imageDescriptors[name].pSampler != pSampler) {
			m_imageDescriptors[name].ptrTexture2DArray = ptrTexture;
			m_imageDescriptors[name].pSampler = (CVKSampler*)pSampler;
			m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
			m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
			m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_bDirty = true;
		}
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		if (m_imageDescriptors[name].ptrTextureCubeMap != ptrTexture || m_imageDescriptors[name].pSampler != pSampler) {
			m_imageDescriptors[name].ptrTextureCubeMap = ptrTexture;
			m_imageDescriptors[name].pSampler = (CVKSampler*)pSampler;
			m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
			m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrTexture->GetTexture();
			m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_bDirty = true;
		}
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler* pSampler)
{
	if (m_pDescriptorLayout->IsTextureValid(name) && m_imageDescriptors.find(name) == m_imageDescriptors.end()) {
		if (m_imageDescriptors[name].ptrRenderTexture != ptrRenderTexture || m_imageDescriptors[name].pSampler != pSampler) {
			m_imageDescriptors[name].ptrRenderTexture = ptrRenderTexture;
			m_imageDescriptors[name].pSampler = (CVKSampler*)pSampler;
			m_imageDescriptors[name].imageInfo.sampler = (VkSampler)pSampler->GetSampler();
			m_imageDescriptors[name].imageInfo.imageView = (VkImageView)ptrRenderTexture->GetTexture();
			m_imageDescriptors[name].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_bDirty = true;
		}
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer)
{
	if (m_pDescriptorLayout->IsUniformBlockValid(name) && m_bufferDescriptors.find(name) == m_bufferDescriptors.end()) {
		if (m_bufferDescriptors[name].ptrUniformBuffer != ptrUniformBuffer) {
			m_bufferDescriptors[name].ptrUniformBuffer = ptrUniformBuffer;
			m_bufferDescriptors[name].bufferInfo.range = ptrUniformBuffer->GetSize();
			m_bufferDescriptors[name].bufferInfo.buffer = (VkBuffer)ptrUniformBuffer->GetBuffer();
			m_bDirty = true;
		}
		return true;
	}
	else {
		return false;
	}
}

CGfxSampler* CVKDescriptorSet::GetSampler(uint32_t name) const
{
	const auto& itImageDescriptor = m_imageDescriptors.find(name);

	if (itImageDescriptor != m_imageDescriptors.end()) {
		return itImageDescriptor->second.pSampler;
	}
	else {
		return nullptr;
	}
}

CGfxTexture2DPtr CVKDescriptorSet::GetTexture2D(uint32_t name) const
{
	const auto& itImageDescriptor = m_imageDescriptors.find(name);

	if (itImageDescriptor != m_imageDescriptors.end()) {
		return itImageDescriptor->second.ptrTexture2D;
	}
	else {
		return nullptr;
	}
}

CGfxTexture2DArrayPtr CVKDescriptorSet::GetTexture2DArray(uint32_t name) const
{
	const auto& itImageDescriptor = m_imageDescriptors.find(name);

	if (itImageDescriptor != m_imageDescriptors.end()) {
		return itImageDescriptor->second.ptrTexture2DArray;
	}
	else {
		return nullptr;
	}
}

CGfxTextureCubeMapPtr CVKDescriptorSet::GetTextureCubeMap(uint32_t name) const
{
	const auto& itImageDescriptor = m_imageDescriptors.find(name);

	if (itImageDescriptor != m_imageDescriptors.end()) {
		return itImageDescriptor->second.ptrTextureCubeMap;
	}
	else {
		return nullptr;
	}
}

CGfxRenderTexturePtr CVKDescriptorSet::GetRenderTexture(uint32_t name) const
{
	const auto& itImageDescriptor = m_imageDescriptors.find(name);

	if (itImageDescriptor != m_imageDescriptors.end()) {
		return itImageDescriptor->second.ptrRenderTexture;
	}
	else {
		return nullptr;
	}
}

void CVKDescriptorSet::Update(void)
{
	if (m_bDirty) {
		m_bDirty = false;

		eastl::vector<VkWriteDescriptorSet> writes;

		for (const auto& itImage : m_imageDescriptors) {
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

		for (const auto& itBuffer : m_bufferDescriptors) {
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
}

void CVKDescriptorSet::Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout) const
{
	eastl::vector<uint32_t> offsets;
	for (const auto& itBuffer : m_bufferDescriptors) {
		offsets.emplace_back(((CVKUniformBuffer*)itBuffer.second.ptrUniformBuffer.GetPointer())->GetBaseOffset());
	}

	vkCmdBindDescriptorSets(vkCommandBuffer, vkPipelineBindPoint, vkPipelineLayout, m_pDescriptorLayout->GetSetIndex(), 1, &m_vkDescriptorSet, offsets.size(), offsets.data());
}
*/