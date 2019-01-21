#include "VKRenderer.h"


CVKImage::CVKImage(CVKDevice *pDevice, VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags)
	: m_pDevice(pDevice)

	, m_vkImage(VK_NULL_HANDLE)
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
	CALL_VK_FUNCTION_RETURN(vkCreateImage(m_pDevice->GetDevice(), &imageCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

	VkMemoryPropertyFlags memoryPropertyFlags = 
		imageTiling == VK_IMAGE_TILING_LINEAR ? 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 
		imageUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ? 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT :
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);

	if (m_pMemory) {
		m_pMemory->BindImage(m_vkImage);
	}
}

CVKImage::~CVKImage(void)
{
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

VkImage CVKImage::GetImage(void) const
{
	return m_vkImage;
}

VkDeviceSize CVKImage::GetSize(void) const
{
	if (m_pMemory) {
		return m_pMemory->GetSize();
	}
	else {
		return 0;
	}
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

bool CVKImage::IsDeviceLocal(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsDeviceLocal();
	}
	else {
		return false;
	}
}

bool CVKImage::IsHostVisible(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsHostVisible();
	}
	else {
		return false;
	}
}

bool CVKImage::IsHostCoherent(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsHostCoherent();
	}
	else {
		return false;
	}
}

bool CVKImage::IsHostCached(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsHostCached();
	}
	else {
		return false;
	}
}

bool CVKImage::IsLazilyAllocated(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsLazilyAllocated();
	}
	else {
		return false;
	}
}
