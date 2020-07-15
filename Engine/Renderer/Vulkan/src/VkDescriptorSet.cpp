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
		if (itImageDescriptorInfo.second.ptrImage2D) {
			SetImage2D(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrImage2D);
		}

		if (itImageDescriptorInfo.second.ptrImage2DArray) {
			SetImage2DArray(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrImage2DArray);
		}

		if (itImageDescriptorInfo.second.ptrImageCubemap) {
			SetImageCubemap(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrImageCubemap);
		}

		if (itImageDescriptorInfo.second.ptrImageRenderTexture) {
			SetImageRenderTexture(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrImageRenderTexture);
		}

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

	for (const auto& itBufferDescriptorInfo : ((CVKDescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_bufferDescriptorInfos) {
		if (itBufferDescriptorInfo.second.ptrUniformBuffer) {
			SetUniformBuffer(itBufferDescriptorInfo.first, itBufferDescriptorInfo.second.ptrUniformBuffer, itBufferDescriptorInfo.second.offset, itBufferDescriptorInfo.second.range);
		}

		if (itBufferDescriptorInfo.second.ptrStorageBuffer) {
			SetStorageBuffer(itBufferDescriptorInfo.first, itBufferDescriptorInfo.second.ptrStorageBuffer, itBufferDescriptorInfo.second.offset, itBufferDescriptorInfo.second.range);
		}
	}
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

bool CVKDescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrBuffer, uint32_t offset, uint32_t range)
{
	ASSERT(ptrBuffer);
	ASSERT(ptrBuffer->GetSize() >= offset + range);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsUniformBlockValid(name)) {
		m_bufferDescriptorInfos[name].SetUniformBuffer(m_ptrDescriptorLayout->GetUniformBlockBinding(name), ptrBuffer, offset, range);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrBuffer, uint32_t offset, uint32_t range)
{
	ASSERT(ptrBuffer);
	ASSERT(ptrBuffer->GetSize() >= offset + range);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsStorageBlockValid(name)) {
		m_bufferDescriptorInfos[name].SetStorageBuffer(m_ptrDescriptorLayout->GetStorageBlockBinding(name), ptrBuffer, offset, range);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImage2D(uint32_t name, const CGfxTexture2DPtr ptrImage)
{
	ASSERT(ptrImage);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImage2D(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImage2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrImage)
{
	ASSERT(ptrImage);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImage2DArray(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImageCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrImage)
{
	ASSERT(ptrImage);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsStorageImageValid(name)) {
		m_imageDescriptorInfos[name].SetImageCubemap(m_ptrDescriptorLayout->GetStorageImageBinding(name), ptrImage);
		return true;
	}
	else {
		return false;
	}
}

bool CVKDescriptorSet::SetImageRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrImage)
{
	ASSERT(ptrImage);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

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
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

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
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

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
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

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
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

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
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

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

static VkDescriptorBufferInfo DescriptorBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range)
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = offset;
	bufferInfo.range = range;
	return bufferInfo;
}

static VkDescriptorImageInfo DescriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout)
{
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.sampler = sampler;
	imageInfo.imageView = imageView;
	imageInfo.imageLayout = imageLayout;
	return imageInfo;
}

static VkWriteDescriptorSet UniformBufferWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorBufferInfo* pBufferInfo)
{
	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = nullptr;
	write.dstSet = dstSet;
	write.dstBinding = dstBinding;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	write.pImageInfo = nullptr;
	write.pBufferInfo = pBufferInfo;
	write.pTexelBufferView = nullptr;
	return write;
}

static VkWriteDescriptorSet StorageBufferWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorBufferInfo* pBufferInfo)
{
	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = nullptr;
	write.dstSet = dstSet;
	write.dstBinding = dstBinding;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	write.pImageInfo = nullptr;
	write.pBufferInfo = pBufferInfo;
	write.pTexelBufferView = nullptr;
	return write;
}

static VkWriteDescriptorSet StorageImageWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorImageInfo* pImageInfo)
{
	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = nullptr;
	write.dstSet = dstSet;
	write.dstBinding = dstBinding;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	write.pImageInfo = pImageInfo;
	write.pBufferInfo = nullptr;
	write.pTexelBufferView = nullptr;
	return write;
}

static VkWriteDescriptorSet SampledImageWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorImageInfo* pImageInfo)
{
	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = nullptr;
	write.dstSet = dstSet;
	write.dstBinding = dstBinding;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.pImageInfo = pImageInfo;
	write.pBufferInfo = nullptr;
	write.pTexelBufferView = nullptr;
	return write;
}

static VkWriteDescriptorSet InputAttachmentWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorImageInfo* pImageInfo)
{
	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = nullptr;
	write.dstSet = dstSet;
	write.dstBinding = dstBinding;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	write.pImageInfo = pImageInfo;
	write.pBufferInfo = nullptr;
	write.pTexelBufferView = nullptr;
	return write;
}

void CVKDescriptorSet::Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout)
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkDescriptorSet);
	ASSERT(m_ptrDescriptorLayout);

	eastl::vector<VkWriteDescriptorSet> writes;
	eastl::vector<VkDescriptorImageInfo> imageInfos;
	eastl::vector<VkDescriptorBufferInfo> bufferInfos;
	writes.reserve(m_imageDescriptorInfos.size() + m_bufferDescriptorInfos.size());
	imageInfos.reserve(m_imageDescriptorInfos.size());
	bufferInfos.reserve(m_bufferDescriptorInfos.size());

	for (auto& itBuffer : m_bufferDescriptorInfos) {
		if (itBuffer.second.bDirty) {
			itBuffer.second.bDirty = false;

			if (itBuffer.second.ptrUniformBuffer) {
				bufferInfos.emplace_back(DescriptorBufferInfo(
					((CVKUniformBuffer*)itBuffer.second.ptrUniformBuffer.GetPointer())->GetBuffer(),
					itBuffer.second.offset,
					itBuffer.second.range));

				writes.emplace_back(UniformBufferWriteDescriptorSet(
					m_vkDescriptorSet,
					itBuffer.second.binding,
					&bufferInfos[bufferInfos.size() - 1]));
			}

			if (itBuffer.second.ptrStorageBuffer) {
				bufferInfos.emplace_back(DescriptorBufferInfo(
					((CVKStorageBuffer*)itBuffer.second.ptrStorageBuffer.GetPointer())->GetBuffer(),
					itBuffer.second.offset,
					itBuffer.second.range));

				writes.emplace_back(StorageBufferWriteDescriptorSet(
					m_vkDescriptorSet,
					itBuffer.second.binding,
					&bufferInfos[bufferInfos.size() - 1]));
			}
		}
	}

	for (auto& itImage : m_imageDescriptorInfos) {
		if (itImage.second.bDirty) {
			itImage.second.bDirty = false;

			if (itImage.second.ptrImage2D) {
				imageInfos.emplace_back(DescriptorImageInfo(
					VK_NULL_HANDLE,
					((CVKTexture2D*)itImage.second.ptrImage2D.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrImage2D->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(StorageImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrImage2DArray) {
				imageInfos.emplace_back(DescriptorImageInfo(
					VK_NULL_HANDLE,
					((CVKTexture2DArray*)itImage.second.ptrImage2DArray.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrImage2DArray->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(StorageImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrImageCubemap) {
				imageInfos.emplace_back(DescriptorImageInfo(
					VK_NULL_HANDLE,
					((CVKTextureCubemap*)itImage.second.ptrImageCubemap.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrImageCubemap->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(StorageImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrImageRenderTexture) {
				imageInfos.emplace_back(DescriptorImageInfo(
					VK_NULL_HANDLE,
					((CVKRenderTexture*)itImage.second.ptrImageRenderTexture.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrImageRenderTexture->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(StorageImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrTexture2D) {
				imageInfos.emplace_back(DescriptorImageInfo(
					((CVKSampler*)itImage.second.pSampler)->GetSampler(),
					((CVKTexture2D*)itImage.second.ptrTexture2D.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrTexture2D->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(SampledImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrTexture2DArray) {
				imageInfos.emplace_back(DescriptorImageInfo(
					((CVKSampler*)itImage.second.pSampler)->GetSampler(),
					((CVKTexture2DArray*)itImage.second.ptrTexture2DArray.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrTexture2DArray->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(SampledImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrTextureCubemap) {
				imageInfos.emplace_back(DescriptorImageInfo(
					((CVKSampler*)itImage.second.pSampler)->GetSampler(),
					((CVKTextureCubemap*)itImage.second.ptrTextureCubemap.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrTextureCubemap->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(SampledImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrRenderTexture) {
				imageInfos.emplace_back(DescriptorImageInfo(
					((CVKSampler*)itImage.second.pSampler)->GetSampler(),
					((CVKRenderTexture*)itImage.second.ptrRenderTexture.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrRenderTexture->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(SampledImageWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}

			if (itImage.second.ptrInputAttachmentTexture) {
				imageInfos.emplace_back(DescriptorImageInfo(
					((CVKSampler*)itImage.second.pSampler)->GetSampler(),
					((CVKRenderTexture*)itImage.second.ptrInputAttachmentTexture.GetPointer())->GetImageView(),
					CGfxHelper::IsFormatDepthOrStencil(itImage.second.ptrInputAttachmentTexture->GetFormat()) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

				writes.emplace_back(InputAttachmentWriteDescriptorSet(
					m_vkDescriptorSet,
					itImage.second.binding,
					&imageInfos[imageInfos.size() - 1]));
			}
		}
	}

	if (writes.empty() == false) {
		vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, nullptr);
	}

	eastl::vector<uint32_t> offsets;
	eastl::unordered_map<uint32_t, uint32_t> orderOffsets;
	offsets.reserve(m_bufferDescriptorInfos.size());
	orderOffsets.reserve(m_bufferDescriptorInfos.size());

	for (const auto& itBuffer : m_bufferDescriptorInfos) {
		if (itBuffer.second.ptrUniformBuffer) {
			orderOffsets[m_ptrDescriptorLayout->GetUniformBlockBinding(itBuffer.first)] = ((CVKUniformBuffer*)itBuffer.second.ptrUniformBuffer.GetPointer())->GetOffset();
		}

		if (itBuffer.second.ptrStorageBuffer) {
			orderOffsets[m_ptrDescriptorLayout->GetStorageBlockBinding(itBuffer.first)] = ((CVKStorageBuffer*)itBuffer.second.ptrStorageBuffer.GetPointer())->GetOffset();
		}
	}

	for (const auto& itOffset : orderOffsets) {
		offsets.emplace_back(itOffset.second);
	}

	vkCmdBindDescriptorSets(vkCommandBuffer, vkPipelineBindPoint, vkPipelineLayout, m_ptrDescriptorLayout->GetSetIndex(), 1, &m_vkDescriptorSet, offsets.size(), offsets.data());
}
