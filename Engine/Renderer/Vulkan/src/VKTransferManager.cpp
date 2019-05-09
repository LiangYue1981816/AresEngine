#include "VKRenderer.h"


CVKTransferManager::CVKTransferManager(CVKDevice* pDevice, uint32_t queueFamilyIndex)
	: m_pDevice(pDevice)

	, m_vkQueue(VK_NULL_HANDLE)
	, m_vkCommandPool(VK_NULL_HANDLE)
{
	vkGetDeviceQueue(m_pDevice->GetDevice(), queueFamilyIndex, 0, &m_vkQueue);

	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	createInfo.queueFamilyIndex = queueFamilyIndex;
	CALL_VK_FUNCTION_RETURN(vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool));
}

CVKTransferManager::~CVKTransferManager(void)
{
	for (const auto& itPending : m_pendingList) {
		delete itPending;
	}

	for (const auto& itFreeList : m_freeLists) {
		for (const auto& itFree : itFreeList.second) {
			delete itFree;
		}
	}

	if (m_vkCommandPool) {
		vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}
}

CVKTransferBuffer* CVKTransferManager::AcquireTransferBuffer(size_t size)
{
	ASSERT(size);
	ASSERT(m_vkQueue);
	ASSERT(m_vkCommandPool);

	for (eastl::vector<CVKTransferBuffer*>::const_iterator itTransferBuffer = m_pendingList.begin(); itTransferBuffer != m_pendingList.end();) {
		CVKTransferBuffer* pTransferBuffer = *itTransferBuffer;

		if (pTransferBuffer->IsTransferFinish()) {
			m_freeLists[pTransferBuffer->GetSize()].emplace_back(pTransferBuffer);
			itTransferBuffer = m_pendingList.erase(itTransferBuffer);
		}
		else {
			itTransferBuffer++;
		}
	}

	CVKTransferBuffer* pTransferBuffer = nullptr;
	{
		if (m_freeLists[size].empty()) {
			pTransferBuffer = new CVKTransferBuffer(m_pDevice, m_vkQueue, m_vkCommandPool, size);
		}
		else {
			pTransferBuffer = m_freeLists[size].front(); m_freeLists[size].pop_front();
		}

		m_pendingList.emplace_back(pTransferBuffer);
	}

	return pTransferBuffer;
}

bool CVKTransferManager::TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data)
{
	if (CVKTransferBuffer* pTransferBuffer = AcquireTransferBuffer(size)) {
		return pTransferBuffer->TransferBufferData(pDstBuffer, dstAccessFlags, dstPipelineStageFlags, offset, size, data);
	}
	else {
		return false;
	}
}

bool CVKTransferManager::TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (CVKTransferBuffer* pTransferBuffer = AcquireTransferBuffer(size)) {
		return pTransferBuffer->TransferTexture2DData(pDstTexture, dstImageLayout, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}

bool CVKTransferManager::TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (CVKTransferBuffer* pTransferBuffer = AcquireTransferBuffer(size)) {
		return pTransferBuffer->TransferTexture2DArrayData(pDstTexture, dstImageLayout, layer, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}

bool CVKTransferManager::TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (CVKTransferBuffer* pTransferBuffer = AcquireTransferBuffer(size)) {
		return pTransferBuffer->TransferTextureCubemapData(pDstTexture, dstImageLayout, face, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}
