#include "VKRenderer.h"


CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	: CGfxDescriptorSet(name, ptrDescriptorLayout)
	, m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)

	, m_vkDescriptorSet(VK_NULL_HANDLE)
{
	Create(ptrDescriptorLayout);
}

CVKDescriptorSet::CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
	: CGfxDescriptorSet(name, ptrDescriptorSetCopyFrom->GetDescriptorLayout())
	, m_pDevice(pDevice)
	, m_pDescriptorPool(pDescriptorPool)

	, m_vkDescriptorSet(VK_NULL_HANDLE)
{
	Create(ptrDescriptorSetCopyFrom->GetDescriptorLayout());

	for (const auto& itImageDescriptorInfo : ((CVKDescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_imageDescriptorInfos) {
		if (itImageDescriptorInfo.second.ptrTexture2D) {
			SetTexture2D(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrTexture2D, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrTexture2DArray) {
			SetTexture2DArray(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrTexture2DArray, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrTextureCubemap) {
			SetTextureCubemap(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrTextureCubemap, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrRenderTexture) {
			SetRenderTexture(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrRenderTexture, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrInputAttachmentTexture) {
			SetInputAttachmentTexture(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrInputAttachmentTexture, itImageDescriptorInfo.second.pSampler);
		}
	}

	for (const auto& itbufferDescriptorInfo : ((CVKDescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_bufferDescriptorInfos) {
		SetUniformBuffer(itbufferDescriptorInfo.first, itbufferDescriptorInfo.second.ptrUniformBuffer, itbufferDescriptorInfo.second.offset, itbufferDescriptorInfo.second.range);
	}

	Update();
}

CVKDescriptorSet::~CVKDescriptorSet(void)
{
	Destroy();
}

void CVKDescriptorSet::Release(void)
{
	m_pDescriptorPool->GetDescriptorSetManager()->Destroy(this);
}

bool CVKDescriptorSet::Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	ASSERT(ptrDescriptorLayout);

	m_ptrDescriptorLayout = ptrDescriptorLayout;

	VkDescriptorSetLayout vkDescriptorSetLayout = ((CVKDescriptorLayout*)ptrDescriptorLayout.GetPointer())->GetDescriptorLayout();
	VkDescriptorSetAllocateInfo descriptorAllocInfo = {};
	descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorAllocInfo.pNext = nullptr;
	descriptorAllocInfo.descriptorPool = m_pDescriptorPool->GetDescriptorPool();
	descriptorAllocInfo.descriptorSetCount = 1;
	descriptorAllocInfo.pSetLayouts = &vkDescriptorSetLayout;
	CALL_VK_FUNCTION_RETURN_BOOL(vkAllocateDescriptorSets(m_pDevice->GetDevice(), &descriptorAllocInfo, &m_vkDescriptorSet));

	return true;
}

void CVKDescriptorSet::Destroy(void)
{
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	m_vkDescriptorSet = VK_NULL_HANDLE;
	m_ptrDescriptorLayout.Release();
}

CVKDescriptorPool* CVKDescriptorSet::GetDescriptorPool(void) const
{
	return m_pDescriptorPool;
}

bool CVKDescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].bDirty = true;
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D = ptrTexture;
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrRenderTexture.Release();
		m_imageDescriptorInfos[name].ptrInputAttachmentTexture.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].bDirty = true;
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray = ptrTexture;
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrRenderTexture.Release();
		m_imageDescriptorInfos[name].ptrInputAttachmentTexture.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].bDirty = true;
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap = ptrTexture;
		m_imageDescriptorInfos[name].ptrRenderTexture.Release();
		m_imageDescriptorInfos[name].ptrInputAttachmentTexture.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].bDirty = true;
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrRenderTexture = ptrTexture;
		m_imageDescriptorInfos[name].ptrInputAttachmentTexture.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].bDirty = true;
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrRenderTexture.Release();
		m_imageDescriptorInfos[name].ptrInputAttachmentTexture = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range)
{
	ASSERT(ptrUniformBuffer);
	ASSERT(ptrUniformBuffer->GetSize() >= offset + range);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsUniformBlockValid(name)) {
		m_bufferDescriptorInfos[name].bDirty = true;
		m_bufferDescriptorInfos[name].offset = offset;
		m_bufferDescriptorInfos[name].range = range;
		m_bufferDescriptorInfos[name].ptrUniformBuffer = ptrUniformBuffer;
		return true;
	}
	else {
		return false;
	}
}

const CGfxDescriptorLayoutPtr CVKDescriptorSet::GetDescriptorLayout(void) const
{
	ASSERT(m_ptrDescriptorLayout);
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
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	eastl::vector<VkWriteDescriptorSet> writes;
	eastl::vector<VkDescriptorImageInfo> imageInfos;
	eastl::vector<VkDescriptorBufferInfo> bufferInfos;

	for (auto& itImage : m_imageDescriptorInfos) {
		if (itImage.second.bDirty) {
			itImage.second.bDirty = false;

			bool bTextureInputAttachment = false;

			if (itImage.second.ptrTexture2D) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler = ((CVKSampler*)itImage.second.pSampler)->GetSampler();
				imageInfo.imageView = ((CVKTexture2D*)itImage.second.ptrTexture2D.GetPointer())->GetImageView();
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageInfos.emplace_back(imageInfo);
				bTextureInputAttachment = false;
			}
			else if (itImage.second.ptrTexture2DArray) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler = ((CVKSampler*)itImage.second.pSampler)->GetSampler();
				imageInfo.imageView = ((CVKTexture2DArray*)itImage.second.ptrTexture2DArray.GetPointer())->GetImageView();
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageInfos.emplace_back(imageInfo);
				bTextureInputAttachment = false;
			}
			else if (itImage.second.ptrTextureCubemap) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler = ((CVKSampler*)itImage.second.pSampler)->GetSampler();
				imageInfo.imageView = ((CVKTextureCubemap*)itImage.second.ptrTextureCubemap.GetPointer())->GetImageView();
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageInfos.emplace_back(imageInfo);
				bTextureInputAttachment = false;
			}
			else if (itImage.second.ptrRenderTexture) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler = ((CVKSampler*)itImage.second.pSampler)->GetSampler();
				imageInfo.imageView = ((CVKRenderTexture*)itImage.second.ptrRenderTexture.GetPointer())->GetImageView();
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageInfos.emplace_back(imageInfo);
				bTextureInputAttachment = false;
			}
			else if (itImage.second.ptrInputAttachmentTexture) {
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.sampler = ((CVKSampler*)itImage.second.pSampler)->GetSampler();
				imageInfo.imageView = ((CVKRenderTexture*)itImage.second.ptrInputAttachmentTexture.GetPointer())->GetImageView();
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageInfos.emplace_back(imageInfo);
				bTextureInputAttachment = true;
			}
			else {
				ASSERT(false);
				continue;
			}

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = bTextureInputAttachment ? m_ptrDescriptorLayout->GetInputAttachmentBinding(itImage.first) : m_ptrDescriptorLayout->GetSampledImageBinding(itImage.first);
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = bTextureInputAttachment ? VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.pImageInfo = &imageInfos[imageInfos.size() - 1];
			write.pBufferInfo = nullptr;
			write.pTexelBufferView = nullptr;
			writes.emplace_back(write);
		}
	}

	for (auto& itBuffer : m_bufferDescriptorInfos) {
		if (itBuffer.second.bDirty) {
			itBuffer.second.bDirty = false;

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = ((CVKUniformBuffer*)itBuffer.second.ptrUniformBuffer.GetPointer())->GetBuffer();
			bufferInfo.offset = itBuffer.second.offset;
			bufferInfo.range = itBuffer.second.range;
			bufferInfos.emplace_back(bufferInfo);

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = m_ptrDescriptorLayout->GetUniformBlockBinding(itBuffer.first);
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			write.pImageInfo = nullptr;
			write.pBufferInfo = &bufferInfos[bufferInfos.size() - 1];
			write.pTexelBufferView = nullptr;
			writes.emplace_back(write);
		}
	}

	if (writes.empty() == false) {
		vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, nullptr);
	}
}

void CVKDescriptorSet::Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	eastl::vector<uint32_t> offsets;

	for (const auto& itBuffer : m_bufferDescriptorInfos) {
		offsets.emplace_back(((CVKUniformBuffer*)itBuffer.second.ptrUniformBuffer.GetPointer())->GetOffset());
	}

	vkCmdBindDescriptorSets(vkCommandBuffer, vkPipelineBindPoint, vkPipelineLayout, m_ptrDescriptorLayout->GetSetIndex(), 1, &m_vkDescriptorSet, offsets.size(), offsets.data());
}
