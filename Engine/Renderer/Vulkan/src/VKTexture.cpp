#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager)
	: m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_bExtern(false)
	, m_vkImageView(VK_NULL_HANDLE)

	, m_transferLevels(1)
	, m_transferLayers(1)
{

}

CVKTexture::~CVKTexture(void)
{

}

bool CVKTexture::CreateView(VkImageView vkImageView)
{
	Destroy();

	m_bExtern = true;
	m_vkImageView = vkImageView;

	return true;
}

bool CVKTexture::CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format, int levels, int layers)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.image = m_ptrImage->GetImage();
	createInfo.viewType = viewType;
	createInfo.format = format;
	createInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	createInfo.subresourceRange = { aspectMask, 0, (uint32_t)levels, 0, (uint32_t)layers };
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));
	return true;
}

bool CVKTexture::CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags)
{
	m_ptrImage = CVKImagePtr(new CVKImage(m_pDevice, imageType, viewType, format, width, height, levels, layers, samples, imageTiling, imageUsageFlags));
	return m_ptrImage->GetImage() != VK_NULL_HANDLE;
}

void CVKTexture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		}
	}

	m_bExtern = false;
	m_vkImageView = VK_NULL_HANDLE;
	m_ptrImage.Release();

	m_transferLevels = 1;
	m_transferLayers = 1;
	m_transferBuffers.clear();
}

void CVKTexture::Bind(VkCommandBuffer vkCommandBuffer, CVKBufferPtr &ptrBufferTransfer)
{
	if (m_transferBuffers.size()) {
		eastl::vector<uint8_t> buffers;
		eastl::vector<VkBufferImageCopy> regions;

		for (auto &itTransferBuffer : m_transferBuffers) {
			itTransferBuffer.second.region.bufferOffset = buffers.size();
			regions.insert(regions.end(), itTransferBuffer.second.region);
			buffers.insert(buffers.end(), itTransferBuffer.second.buffer.data(), itTransferBuffer.second.buffer.data() + itTransferBuffer.second.buffer.size());
		}

		ptrBufferTransfer = CVKBufferPtr(new CVKBuffer(m_pDevice, buffers.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		ptrBufferTransfer->BufferData(0, buffers.size(), buffers.data());
		vkCmdTransferImage(vkCommandBuffer, ptrBufferTransfer->GetBuffer(), m_ptrImage->GetImage(), m_transferLevels, m_transferLayers, regions.data(), regions.size());

		m_transferLevels = 1;
		m_transferLayers = 1;
		m_transferBuffers.clear();
	}
}

VkImageView CVKTexture::GetImageView(void) const
{
	return m_vkImageView;
}
