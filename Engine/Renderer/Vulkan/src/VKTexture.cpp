#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_bExtern(false)

	, m_vkImage(VK_NULL_HANDLE)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_vkImageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
	, m_vkImageAspectFlags(0)

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

VkImage CVKTexture::GetImage(void) const
{
	return m_vkImage;
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

bool CVKTexture::Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImageView vkImageView)
{
	Destroy();

	m_bExtern = true;
	m_pMemory = nullptr;

	m_vkImage = VK_NULL_HANDLE;
	m_vkImageView = vkImageView;
	m_vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_vkImageAspectFlags = imageAspectFlags;

	m_type = type;
	m_format = format;

	m_width = width;
	m_height = height;
	m_layers = layers;
	m_levels = levels;
	m_samples = samples;

	return true;
}

bool CVKTexture::Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImageUsageFlags imageUsageFlags, VkImageTiling imageTiling)
{
	Destroy();
	{
		do {
			m_bExtern = false;
			m_pMemory = nullptr;

			m_vkImage = VK_NULL_HANDLE;
			m_vkImageView = VK_NULL_HANDLE;
			m_vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
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
			CALL_VK_FUNCTION_BREAK(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.image = m_vkImage;
			createInfo.viewType = CVKHelper::TranslateImageViewType(type);
			createInfo.format = (VkFormat)format;
			createInfo.components = CVKHelper::GetFormatComponentMapping((VkFormat)format);
			createInfo.subresourceRange = { imageAspectFlags, 0, (uint32_t)levels, 0, (uint32_t)layers };
			CALL_VK_FUNCTION_BREAK(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));

			VkMemoryPropertyFlags memoryPropertyFlags =
				imageTiling == VK_IMAGE_TILING_LINEAR ?
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT :
				imageUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ?
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT :
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

			VkMemoryRequirements requirements;
			vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);

			m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
			if (m_pMemory == nullptr) break;
			if (m_pMemory->BindImage(m_vkImage) == false) break;

			return true;
		} while (false);
	}
	Destroy();
	return false;
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
	m_pMemory = nullptr;

	m_vkImage = VK_NULL_HANDLE;
	m_vkImageView = VK_NULL_HANDLE;
	m_vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_vkImageAspectFlags = 0;

	m_type = GFX_TEXTURE_INVALID_ENUM;
	m_format = GFX_PIXELFORMAT_UNDEFINED;

	m_width = 0;
	m_height = 0;
	m_layers = 0;
	m_levels = 0;
	m_samples = 0;
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

	return m_pDevice->GetTransferManager()->TransferTexture2DData(this, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, level, xoffset, yoffset, width, height, size, data);
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

	return m_pDevice->GetTransferManager()->TransferTexture2DArrayData(this, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, layer, level, xoffset, yoffset, width, height, size, data);
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

	return m_pDevice->GetTransferManager()->TransferTextureCubemapData(this, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, face, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTexture::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout, VkImageSubresourceRange range)
{
	if (m_vkImage) {
		CALL_VK_FUNCTION_RETURN_BOOL(vkCmdImageMemoryBarrier(vkCommandBuffer, m_vkImage, m_vkImageLayout, imageLayout, range));
		m_vkImageLayout = imageLayout;
		return true;
	}
	else {
		return false;
	}
}

bool CVKTexture::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkImageSubresourceRange range)
{
	if (m_vkImage) {
		CALL_VK_FUNCTION_RETURN_BOOL(vkCmdImageMemoryBarrier(vkCommandBuffer, m_vkImage, m_vkImageLayout, imageLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, range));
		m_vkImageLayout = imageLayout;
		return true;
	}
	else {
		return false;
	}
}
