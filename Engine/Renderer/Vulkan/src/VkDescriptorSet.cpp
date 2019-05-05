#include "VKRenderer.h"


CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	: CGfxDescriptorSet(ptrDescriptorLayout)
	, m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)
	, m_ptrDescriptorLayout(ptrDescriptorLayout)
	, m_vkDescriptorSet(VK_NULL_HANDLE)

	, m_bDirty(false)
{
	VkDescriptorSetLayout vkDescriptorSetLayout = ((CVKDescriptorLayout*)ptrDescriptorLayout.GetPointer())->GetDescriptorLayout();
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = pDescriptorPool->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &vkDescriptorSetLayout;
	vkAllocateDescriptorSets(m_pDevice->GetDevice(), &allocInfo, &m_vkDescriptorSet);
}

CVKDescriptorSet::~CVKDescriptorSet(void)
{

}

void CVKDescriptorSet::Release(void)
{
	m_pDescriptorPool->FreeDescriptorSet(this);
}

CVKDescriptorPool* CVKDescriptorSet::GetDescriptorPool(void) const
{
	return m_pDescriptorPool;
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D = ptrTexture;
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrTextureInputAttachment.Release();
		m_bDirty = true;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray = ptrTexture;
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrTextureInputAttachment.Release();
		m_bDirty = true;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap = ptrTexture;
		m_imageDescriptorInfos[name].ptrTextureInputAttachment.Release();
		m_bDirty = true;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureInputAttachment(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrTextureInputAttachment = ptrTexture;
		m_bDirty = true;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range)
{
	if (m_ptrDescriptorLayout->IsUniformBlockValid(name)) {
		m_bufferDescriptorInfos[name].offset = offset;
		m_bufferDescriptorInfos[name].range = range;
		m_bufferDescriptorInfos[name].ptrUniformBuffer = ptrUniformBuffer;
		m_bDirty = true;
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

void CVKDescriptorSet::Update(void)
{
	if (m_bDirty) {
		m_bDirty = false;

		eastl::vector<VkWriteDescriptorSet> writes;
		eastl::vector<VkDescriptorImageInfo> imageInfos;
		eastl::vector<VkDescriptorBufferInfo> bufferInfos;

		for (const auto& itImage : m_imageDescriptorInfos) {
			if (itImage.second.ptrTexture2D) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler;
				imageInfo.imageView = ((CVKTexture2D*)itImage.second.ptrTexture2D.GetPointer())->GetImageView();
				imageInfo.imageLayout = ((CVKTexture2D*)itImage.second.ptrTexture2D.GetPointer())->GetImageLayout();
				imageInfos.emplace_back(imageInfo);
			}

			if (itImage.second.ptrTexture2DArray) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler;
				imageInfo.imageView = ((CVKTexture2DArray*)itImage.second.ptrTexture2DArray.GetPointer())->GetImageView();
				imageInfo.imageLayout = ((CVKTexture2DArray*)itImage.second.ptrTexture2DArray.GetPointer())->GetImageLayout();
				imageInfos.emplace_back(imageInfo);
			}

			if (itImage.second.ptrTextureCubemap) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler;
				imageInfo.imageView = ((CVKTextureCubemap*)itImage.second.ptrTextureCubemap.GetPointer())->GetImageView();
				imageInfo.imageLayout = ((CVKTextureCubemap*)itImage.second.ptrTextureCubemap.GetPointer())->GetImageLayout();
				imageInfos.emplace_back(imageInfo);
			}

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = m_ptrDescriptorLayout->GetSampledImageBinding(itImage.first);
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.pImageInfo = &imageInfos[imageInfos.size() - 1];
			write.pBufferInfo = nullptr;
			write.pTexelBufferView = nullptr;
			writes.emplace_back(write);
		}
		
		for (const auto& itImage : m_inputAttachmentDescriptorInfos) {
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.sampler;
			imageInfo.imageView;
			imageInfo.imageLayout;
			imageInfos.emplace_back(imageInfo);

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = m_ptrDescriptorLayout->GetInputAttachmentBinding(itImage.first);
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
			write.pImageInfo = &imageInfos[imageInfos.size() - 1];
			write.pBufferInfo = nullptr;
			write.pTexelBufferView = nullptr;
			writes.emplace_back(write);
		}

		for (const auto& itBuffer : m_bufferDescriptorInfos) {
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer;
			bufferInfo.offset;
			bufferInfo.range;
			bufferInfos.emplace_back(bufferInfo);

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = m_ptrDescriptorLayout->GetUniformBlockBinding(itBuffer.first);
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.pImageInfo = nullptr;
			write.pBufferInfo = &bufferInfos[bufferInfos.size() - 1];
			write.pTexelBufferView = nullptr;
			writes.emplace_back(write);
		}

		vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, nullptr);
	}
}

void CVKDescriptorSet::Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout) const
{
	eastl::vector<uint32_t> offsets;

	for (const auto& itBuffer : m_bufferDescriptorInfos) {
		offsets.emplace_back(((CVKUniformBuffer*)itBuffer.second.ptrUniformBuffer.GetPointer())->GetOffset());
	}

	vkCmdBindDescriptorSets(vkCommandBuffer, vkPipelineBindPoint, vkPipelineLayout, m_ptrDescriptorLayout->GetSetIndex(), 1, &m_vkDescriptorSet, offsets.size(), offsets.data());
}

/*
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