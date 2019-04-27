#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice* pDevice)
	: m_pDevice(pDevice)

	, m_bExtern(false)
	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_vkImageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
	, m_vkImageAspectFlags(VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM)
	, m_pMemory(nullptr)

	, m_type(GFX_TEXTURE_INVALID_ENUM)
	, m_format(GFX_PIXELFORMAT_UNDEFINED)

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

VkImageLayout CVKTexture::GetImageLayout(void) const
{
	return m_vkImageLayout;
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

bool CVKTexture::Create(GfxTextureType type, VkImageView vkImageView, int width, int height, int layers, int levels, int samples)
{
	Destroy();

	m_bExtern = true;
	m_vkImage = VK_NULL_HANDLE;
	m_vkImageView = vkImageView;
	m_vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_vkImageAspectFlags = VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
	m_pMemory = nullptr;

	m_type = type;
	m_format = GFX_PIXELFORMAT_UNDEFINED;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;
	m_samples = samples;

	return true;
}

bool CVKTexture::Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkImageAspectFlags imageAspectFlags)
{
	Destroy();

	m_bExtern = false;
	m_vkImage = VK_NULL_HANDLE;
	m_vkImageView = VK_NULL_HANDLE;
	m_vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_vkImageAspectFlags = imageAspectFlags;
	m_pMemory = nullptr;

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
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.image = m_vkImage;
	createInfo.viewType = CVKHelper::TranslateImageViewType(type);
	createInfo.format = (VkFormat)format;
	createInfo.components = CVKHelper::GetFormatComponentMapping((VkFormat)format);
	createInfo.subresourceRange = { imageAspectFlags, 0, (uint32_t)levels, 0, (uint32_t)layers };
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));

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
	if (m_bExtern == false) {
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		}
	}

	if (m_vkImage) {
		vkDestroyImage(m_pDevice->GetDevice(), m_vkImage, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}

	m_bExtern = false;
	m_vkImage = VK_NULL_HANDLE;
	m_vkImageView = VK_NULL_HANDLE;
	m_vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_vkImageAspectFlags = VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
	m_pMemory = nullptr;

	m_type = GFX_TEXTURE_INVALID_ENUM;
	m_format = GFX_PIXELFORMAT_UNDEFINED;

	m_width = 0;
	m_height = 0;
	m_layers = 0;
	m_levels = 0;
	m_samples = 0;

	m_transferRegions.clear();
	m_transferBuffers.clear();
	m_transferRegions.shrink_to_fit();
	m_transferBuffers.shrink_to_fit();
}

bool CVKTexture::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_type != GFX_TEXTURE_2D) {
		return false;
	}

	if (m_vkImage == VK_NULL_HANDLE) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	VkBufferImageCopy region = {};
	region.bufferOffset = m_transferBuffers.size();
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = m_vkImageAspectFlags;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	m_transferRegions.insert(m_transferRegions.end(), region);
	m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);

	return true;
}

bool CVKTexture::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_type != GFX_TEXTURE_2D) {
		return false;
	}

	if (m_vkImage == VK_NULL_HANDLE) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	VkBufferImageCopy region = {};
	region.bufferOffset = m_transferBuffers.size();
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = m_vkImageAspectFlags;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	m_transferRegions.insert(m_transferRegions.end(), region);
	m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);

	return true;
}

bool CVKTexture::Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_type != GFX_TEXTURE_2D_ARRAY) {
		return false;
	}

	if (m_vkImage == VK_NULL_HANDLE) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	VkBufferImageCopy region = {};
	region.bufferOffset = m_transferBuffers.size();
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = m_vkImageAspectFlags;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = layer;
	region.imageSubresource.layerCount = 1;

	m_transferRegions.insert(m_transferRegions.end(), region);
	m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);

	return true;
}

bool CVKTexture::Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_type != GFX_TEXTURE_2D_ARRAY) {
		return false;
	}

	if (m_vkImage == VK_NULL_HANDLE) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_layers < layer) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	VkBufferImageCopy region = {};
	region.bufferOffset = m_transferBuffers.size();
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = m_vkImageAspectFlags;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = layer;
	region.imageSubresource.layerCount = 1;

	m_transferRegions.insert(m_transferRegions.end(), region);
	m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);

	return true;
}

bool CVKTexture::TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_type != GFX_TEXTURE_CUBE_MAP) {
		return false;
	}

	if (m_vkImage == VK_NULL_HANDLE) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	VkBufferImageCopy region = {};
	region.bufferOffset = m_transferBuffers.size();
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = m_vkImageAspectFlags;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	m_transferRegions.insert(m_transferRegions.end(), region);
	m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);

	return true;
}

bool CVKTexture::TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_bExtern == true) {
		return false;
	}

	if (m_type != GFX_TEXTURE_CUBE_MAP) {
		return false;
	}

	if (m_vkImage == VK_NULL_HANDLE) {
		return false;
	}

	if (m_format != format) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	VkBufferImageCopy region = {};
	region.bufferOffset = m_transferBuffers.size();
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = m_vkImageAspectFlags;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	m_transferRegions.insert(m_transferRegions.end(), region);
	m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);

	return true;
}

bool CVKTexture::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout srcLayout, VkImageLayout dstLayout, VkImageSubresourceRange range)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkCmdImageMemoryBarrier(vkCommandBuffer, m_vkImage, srcLayout, dstLayout, range));
	m_vkImageLayout = dstLayout;
	return true;
}

bool CVKTexture::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout srcLayout, VkImageLayout dstLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkImageSubresourceRange range)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkCmdImageMemoryBarrier(vkCommandBuffer, m_vkImage, srcLayout, dstLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, range));
	m_vkImageLayout = dstLayout;
	return true;
}
