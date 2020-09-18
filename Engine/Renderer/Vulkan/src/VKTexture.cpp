#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice* pDevice, GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImage vkImage)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_bExtern(false)

	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_vkImageAspectFlags(0)

	, m_type(GFX_TEXTURE_INVALID_ENUM)
	, m_format(GFX_PIXELFORMAT_UNDEFINED)

	, m_width(0)
	, m_height(0)
	, m_layers(0)
	, m_levels(0)
	, m_samples(0)
{
	ASSERT(vkImage);
	ASSERT(imageAspectFlags);

	m_bExtern = true;

	m_vkImage = vkImage;
	m_vkImageView = VK_NULL_HANDLE;
	m_vkImageAspectFlags = imageAspectFlags;

	m_type = type;
	m_format = format;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;
	m_samples = samples;

	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.pNext = nullptr;
	viewCreateInfo.flags = 0;
	viewCreateInfo.image = vkImage;
	viewCreateInfo.viewType = CVKHelper::TranslateImageViewType(type);
	viewCreateInfo.format = (VkFormat)format;
	viewCreateInfo.components = CVKHelper::GetFormatComponentMapping((VkFormat)format);
	viewCreateInfo.subresourceRange = { imageAspectFlags, 0, (uint32_t)levels, 0, (uint32_t)layers };
	CALL_VK_FUNCTION_ASSERT(vkCreateImageView(m_pDevice->GetDevice(), &viewCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));
}

CVKTexture::CVKTexture(CVKDevice* pDevice, GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImageUsageFlags imageUsageFlags, VkImageTiling imageTiling)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_bExtern(false)

	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_vkImageAspectFlags(0)

	, m_type(GFX_TEXTURE_INVALID_ENUM)
	, m_format(GFX_PIXELFORMAT_UNDEFINED)

	, m_width(0)
	, m_height(0)
	, m_layers(0)
	, m_levels(0)
	, m_samples(0)
{
	ASSERT(imageUsageFlags);
	ASSERT(imageAspectFlags);

	m_bExtern = false;

	m_vkImage = VK_NULL_HANDLE;
	m_vkImageView = VK_NULL_HANDLE;
	m_vkImageAspectFlags = imageAspectFlags;

	m_type = type;
	m_format = format;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;
	m_samples = samples;

	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = (VkFormat)format;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = levels;
	imageCreateInfo.arrayLayers = layers;
	imageCreateInfo.samples = CVKHelper::TranslateSampleCount(samples);
	imageCreateInfo.tiling = imageTiling;
	imageCreateInfo.usage = imageUsageFlags;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	switch (type) {
	case GFX_TEXTURE_CUBE_MAP: imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT; break;
	case GFX_TEXTURE_2D_ARRAY: imageCreateInfo.flags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT; break;
	}
	CALL_VK_FUNCTION_ASSERT(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.pNext = nullptr;
	viewCreateInfo.flags = 0;
	viewCreateInfo.image = m_vkImage;
	viewCreateInfo.viewType = CVKHelper::TranslateImageViewType(type);
	viewCreateInfo.format = (VkFormat)format;
	viewCreateInfo.components = CVKHelper::GetFormatComponentMapping((VkFormat)format);
	viewCreateInfo.subresourceRange = { imageAspectFlags, 0, (uint32_t)levels, 0, (uint32_t)layers };
	CALL_VK_FUNCTION_ASSERT(vkCreateImageView(m_pDevice->GetDevice(), &viewCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));

	VkMemoryPropertyFlags memoryPropertyFlags =
		imageTiling == VK_IMAGE_TILING_LINEAR ?
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT :
		imageUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ?
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT :
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);

	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, memoryPropertyFlags, imageTiling == VK_IMAGE_TILING_LINEAR ? VK_RESOURCE_TYPE_IMAGE_LINEAR : VK_RESOURCE_TYPE_IMAGE_OPTIMAL);

	if (m_pMemory) {
		m_pMemory->BindImage(m_vkImage);
		CGfxProfiler::IncTextureSize(m_type, m_pMemory->GetSize());
	}
}

CVKTexture::~CVKTexture(void)
{
	if (m_vkImageView) {
		vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_bExtern == false) {
		if (m_vkImage) {
			vkDestroyImage(m_pDevice->GetDevice(), m_vkImage, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_pMemory) {
			CGfxProfiler::DecTextureSzie(m_type, m_pMemory->GetSize());
			m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
		}
	}
}

VkImage CVKTexture::GetImage(void) const
{
	return m_vkImage;
}

VkImageView CVKTexture::GetImageView(void) const
{
	return m_vkImageView;
}

VkImageAspectFlags CVKTexture::GetImageAspectFlags(void) const
{
	return m_vkImageAspectFlags;
}

GfxTextureType CVKTexture::GetType(void) const
{
	return m_type;
}

GfxPixelFormat CVKTexture::GetFormat(void) const
{
	return m_format;
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
