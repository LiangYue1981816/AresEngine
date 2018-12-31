#include "VKRenderer.h"


CVKImage::CVKImage(CVKDevice *pDevice, VkImageType imageType, VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage)
	: m_pDevice(pDevice)

	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_pMemory(nullptr)
{
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = imageType;
	imageCreateInfo.format = format;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = levels;
	imageCreateInfo.arrayLayers = layers;
	imageCreateInfo.samples = samples;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.usage = usage;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	CALL_VK_FUNCTION_RETURN(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

	VkMemoryPropertyFlags memoryPropertyFlags;
	memoryPropertyFlags = tiling == VK_IMAGE_TILING_LINEAR ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
	m_pMemory->BindImage(m_vkImage);

	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.pNext = nullptr;
	viewCreateInfo.flags = 0;
	viewCreateInfo.image = m_vkImage;
	viewCreateInfo.viewType = viewType;
	viewCreateInfo.format = format;
	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	viewCreateInfo.subresourceRange.aspectMask = aspectMask;
	viewCreateInfo.subresourceRange.baseMipLevel = 0;
	viewCreateInfo.subresourceRange.levelCount = levels;
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
	viewCreateInfo.subresourceRange.layerCount = layers;
	CALL_VK_FUNCTION_RETURN(vkCreateImageView(m_pDevice->GetDevice(), &viewCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));
}

CVKImage::~CVKImage(void)
{
	if (m_vkImageView) {
		vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkImage) {
		vkDestroyImage(m_pDevice->GetDevice(), m_vkImage, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}
}

void CVKImage::Release(void)
{
	delete this;
}

bool CVKImage::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_pMemory == nullptr) {
		return false;
	}

	void *pAddress = nullptr;
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(offset, size, &pAddress));
	{
		memcpy(pAddress, pBuffer, size);
	}
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->Flush(offset, size));
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

	return true;
}
