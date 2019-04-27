#include "VKRenderer.h"


CVKTransferBuffer::CVKTransferBuffer(CVKDevice* pDevice, VkQueue vkQueue, VkCommandPool vkCommandPool, VkDeviceSize size)
	: m_pDevice(pDevice)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_pMemory(nullptr)

	, m_vkQueue(vkQueue)
	, m_vkFence(VK_NULL_HANDLE)

	, m_vkCommandPool(vkCommandPool)
	, m_vkCommandBuffer(VK_NULL_HANDLE)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = nullptr;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;
	vkCreateBuffer(m_pDevice->GetDevice(), &bufferCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer);

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	m_pMemory->BindBuffer(m_vkBuffer);

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	vkCreateFence(m_pDevice->GetDevice(), &fenceCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence);

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.commandPool = m_vkCommandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;
	vkAllocateCommandBuffers(m_pDevice->GetDevice(), &allocateInfo, &m_vkCommandBuffer);
}

CVKTransferBuffer::~CVKTransferBuffer(void)
{
	vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX);

	vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	vkFreeCommandBuffers(m_pDevice->GetDevice(), m_vkCommandPool, 1, &m_vkCommandBuffer);

	vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
}

VkDeviceSize CVKTransferBuffer::GetSize(void) const
{
	return m_pMemory->GetSize();
}

bool CVKTransferBuffer::IsTransferFinish(void) const
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	return false;
}

bool CVKTransferBuffer::TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}

bool CVKTransferBuffer::TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}

bool CVKTransferBuffer::TransferTexture2DDataCompressed(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}

bool CVKTransferBuffer::TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}

bool CVKTransferBuffer::TransferTexture2DArrayDataCompressed(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}

bool CVKTransferBuffer::TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}

bool CVKTransferBuffer::TransferTextureCubemapDataCompressed(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{

	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));
	return true;
}
