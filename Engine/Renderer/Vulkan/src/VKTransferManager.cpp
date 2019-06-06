#include "VKRenderer.h"


CVKTransferManager::CVKTransferManager(CVKDevice* pDevice, uint32_t queueFamilyIndex)
	: m_pDevice(pDevice)

	, m_vkQueue(VK_NULL_HANDLE)
	, m_vkCommandPool(VK_NULL_HANDLE)
{
	pthread_mutex_init(&lock, nullptr);

	VkCommandPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolCreateInfo.pNext = nullptr;
	poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	CALL_VK_FUNCTION_RETURN(vkCreateCommandPool(m_pDevice->GetDevice(), &poolCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool));

	vkGetDeviceQueue(m_pDevice->GetDevice(), queueFamilyIndex, 0, &m_vkQueue);
}

CVKTransferManager::~CVKTransferManager(void)
{
	ASSERT(m_vkQueue);
	ASSERT(m_vkCommandPool);

	pthread_mutex_destroy(&lock);

	for (const auto& itTransferBuffer : m_pTransferBuffers) {
		delete itTransferBuffer.second;
	}

	vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
}

CVKTransferBuffer* CVKTransferManager::AcquireTransferBuffer(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(size);
		ASSERT(m_vkQueue);
		ASSERT(m_vkCommandPool);

		size = ALIGN_BYTE(size, 4 * 1024 * 1024);

		if (m_pTransferBuffers.find(size) == m_pTransferBuffers.end()) {
			m_pTransferBuffers[size] = new CVKTransferBuffer(m_pDevice, m_vkQueue, m_vkCommandPool, size);
		}

		return m_pTransferBuffers[size];
	}
}

bool CVKTransferManager::TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data)
{
	return AcquireTransferBuffer(size)->TransferBufferData(pDstBuffer, dstAccessFlags, dstPipelineStageFlags, offset, size, data);
}

bool CVKTransferManager::TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return AcquireTransferBuffer(size)->TransferTexture2DData(pDstTexture, dstImageLayout, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTransferManager::TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return AcquireTransferBuffer(size)->TransferTexture2DArrayData(pDstTexture, dstImageLayout, layer, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTransferManager::TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return AcquireTransferBuffer(size)->TransferTextureCubemapData(pDstTexture, dstImageLayout, face, level, xoffset, yoffset, width, height, size, data);
}
