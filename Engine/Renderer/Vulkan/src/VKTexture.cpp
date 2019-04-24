#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice* pDevice)
	: m_pDevice(pDevice)

	: m_bExtern(false)
	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_pMemory(nullptr)

	, m_width(0)
	, m_height(0)
	, m_layers(0)
	, m_levels(0)
	, m_samples(0)
{

}

CVKTexture::~CVKTexture(void)
{
	Destroy();
}

void CVKTexture::Release(void)
{
	delete this;
}

VkImageView CVKTexture::GetImageView(void) const
{
	return m_vkImageView;
}

int CVKTexture::GetWidth(void) const
{
	return m_width;
}

int CVKTexture::GetHeight(void) const
{
	return m_height;
}

int CVKTexture::GetLayers(void) const
{
	return m_layers;
}

int CVKTexture::GetLevels(void) const
{
	return m_levels;
}

int CVKTexture::GetSamples(void) const
{
	return m_samples;
}

bool CVKTexture::CreateView(VkImageView vkImageView)
{
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
	createInfo.image = m_vkImage;
	createInfo.viewType = viewType;
	createInfo.format = format;
	createInfo.components = CVKHelper::GetFormatComponentMapping(format);
	createInfo.subresourceRange = { aspectMask, 0, (uint32_t)levels, 0, (uint32_t)layers };
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));
	return true;
}

bool CVKTexture::CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int layers, int levels, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags)
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
	imageCreateInfo.tiling = imageTiling;
	imageCreateInfo.usage = imageUsageFlags;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	switch (viewType) {
	case VK_IMAGE_VIEW_TYPE_CUBE: imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT; break;
	case VK_IMAGE_VIEW_TYPE_2D_ARRAY: imageCreateInfo.flags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT; break;
	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

	VkMemoryPropertyFlags memoryPropertyFlags =
		imageTiling == VK_IMAGE_TILING_LINEAR ?
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT :
		imageUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ?
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT :
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
	m_pMemory->BindImage(m_vkImage);

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

bool CVKTexture::TransferTexture2D(int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data)
{
	return true;
}

bool CVKTexture::TransferTexture2DCompressed(int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTexture::TransferTexture2DArray(int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data)
{
	return true;
}

bool CVKTexture::TransferTexture2DArrayCompressed(int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTexture::TransferTextureCubeMap(GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data)
{
	return true;
}

bool CVKTexture::TransferTextureCubeMapCompressed(GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}
