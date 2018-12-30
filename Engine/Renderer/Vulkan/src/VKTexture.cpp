#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager)
	: m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_pMemory(nullptr)
{

}

CVKTexture::~CVKTexture(void)
{

}

bool CVKTexture::Create(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageUsageFlags usage)
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
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = usage;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BindImage(m_vkImage));

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
	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	viewCreateInfo.subresourceRange.baseMipLevel = 0;
	viewCreateInfo.subresourceRange.levelCount = levels;
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
	viewCreateInfo.subresourceRange.layerCount = layers;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImageView(m_pDevice->GetDevice(), &viewCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));

	return true;
}

void CVKTexture::Destroy(void)
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
