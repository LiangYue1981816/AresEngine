#include "VKRenderer.h"


CVKTransferBuffer::CVKTransferBuffer(CVKDevice* pDevice, VkDeviceSize size)
	: m_pDevice(pDevice)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_pMemory(nullptr)
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = size;
	createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer);

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	m_pMemory->BindBuffer(m_vkBuffer);
}

CVKTransferBuffer::~CVKTransferBuffer(void)
{
	vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
}

bool CVKTransferBuffer::TransferBufferData(CVKBuffer* pTransferDstBuffer, size_t offset, size_t size, const void* data) const
{
	return true;
}

bool CVKTransferBuffer::TransferTexture2DData(CVKTexture* pTransferDstTexture, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBuffer::TransferTexture2DDataCompressed(CVKTexture* pTransferDstTexture, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBuffer::TransferTexture2DArrayData(CVKTexture* pTransferDstTexture, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBuffer::TransferTexture2DArrayDataCompressed(CVKTexture* pTransferDstTexture, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBuffer::TransferTextureCubemapData(CVKTexture* pTransferDstTexture, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBuffer::TransferTextureCubemapDataCompressed(CVKTexture* pTransferDstTexture, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}
