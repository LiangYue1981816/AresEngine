#include "VKRenderer.h"


CVKTransferBuffer::CVKTransferBuffer(CVKDevice* pDevice, VkQueue vkQueue, VkCommandPool vkCommandPool, VkDeviceSize size)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkSize(0)

	, m_vkQueue(VK_NULL_HANDLE)
	, m_vkFence(VK_NULL_HANDLE)

	, m_vkCommandPool(VK_NULL_HANDLE)
	, m_vkCommandBuffer(VK_NULL_HANDLE)
{
	Create(vkQueue, vkCommandPool, size);
	CGfxProfiler::IncTransferBufferSize(GetSize());
}

CVKTransferBuffer::~CVKTransferBuffer(void)
{
	CGfxProfiler::DecTransferBufferSize(GetSize());
	Destroy();
}

bool CVKTransferBuffer::Create(VkQueue vkQueue, VkCommandPool vkCommandPool, VkDeviceSize size)
{
	ASSERT(size);
	ASSERT(vkQueue);
	ASSERT(vkCommandPool);

	m_vkSize = size;
	m_vkQueue = vkQueue;
	m_vkCommandPool = vkCommandPool;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateFence(m_pDevice->GetDevice(), &fenceCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence));

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = vkCommandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;
	CALL_VK_FUNCTION_RETURN_BOOL(vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferAllocateInfo, &m_vkCommandBuffer));

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = nullptr;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateBuffer(m_pDevice->GetDevice(), &bufferCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);

	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	if (m_pMemory == nullptr) { ASSERT(false); return false; }
	if (m_pMemory->BindBuffer(m_vkBuffer) == false) { ASSERT(false); return false; }

	return true;
}

void CVKTransferBuffer::Destroy(void)
{
	ASSERT(m_pMemory);
	ASSERT(m_vkFence);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkCommandBuffer);

	vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX);
	vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);

	vkFreeCommandBuffers(m_pDevice->GetDevice(), m_vkCommandPool, 1, &m_vkCommandBuffer);

	m_pMemory = nullptr;
	m_vkBuffer = VK_NULL_HANDLE;
	m_vkSize = 0;

	m_vkQueue = VK_NULL_HANDLE;
	m_vkFence = VK_NULL_HANDLE;

	m_vkCommandPool = VK_NULL_HANDLE;
	m_vkCommandBuffer = VK_NULL_HANDLE;
}

VkDeviceSize CVKTransferBuffer::GetSize(void) const
{
	return m_vkSize;
}

VkDeviceSize CVKTransferBuffer::GetMemorySize(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->GetSize();
}

bool CVKTransferBuffer::TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data)
{
	ASSERT(pDstBuffer);
	ASSERT(m_vkFence);
	ASSERT(m_pMemory);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkCommandBuffer);

	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

		VkBufferCopy region = {};
		region.srcOffset = 0;
		region.dstOffset = offset;
		region.size = size;
		vkCmdCopyBuffer(m_vkCommandBuffer, m_vkBuffer, pDstBuffer->GetBuffer(), 1, &region);

		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstBuffer->PipelineBarrier(m_vkCommandBuffer, VK_ACCESS_TRANSFER_WRITE_BIT, dstAccessFlags, VK_PIPELINE_STAGE_TRANSFER_BIT, dstPipelineStageFlags, offset, size));
	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));

	return true;
}

bool CVKTransferBuffer::TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(pDstTexture);
	ASSERT(m_vkFence);
	ASSERT(m_pMemory);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkCommandBuffer);

	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.imageOffset.x = xoffset;
		region.imageOffset.y = yoffset;
		region.imageOffset.z = 0;
		region.imageExtent.width = width;
		region.imageExtent.height = height;
		region.imageExtent.depth = 1;
		region.imageSubresource.aspectMask = pDstTexture->GetImageAspectFlags();
		region.imageSubresource.mipLevel = level;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstTexture->PipelineBarrier(m_vkCommandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL));
		{
			vkCmdCopyBufferToImage(m_vkCommandBuffer, m_vkBuffer, pDstTexture->GetImage(), pDstTexture->GetImageLayout(), 1, &region);
		}
		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstTexture->PipelineBarrier(m_vkCommandBuffer, dstImageLayout));
	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));

	return true;
}

bool CVKTransferBuffer::TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(pDstTexture);
	ASSERT(m_vkFence);
	ASSERT(m_pMemory);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkCommandBuffer);

	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.imageOffset.x = xoffset;
		region.imageOffset.y = yoffset;
		region.imageOffset.z = 0;
		region.imageExtent.width = width;
		region.imageExtent.height = height;
		region.imageExtent.depth = 1;
		region.imageSubresource.aspectMask = pDstTexture->GetImageAspectFlags();
		region.imageSubresource.mipLevel = level;
		region.imageSubresource.baseArrayLayer = layer;
		region.imageSubresource.layerCount = 1;

		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstTexture->PipelineBarrier(m_vkCommandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL));
		{
			vkCmdCopyBufferToImage(m_vkCommandBuffer, m_vkBuffer, pDstTexture->GetImage(), pDstTexture->GetImageLayout(), 1, &region);
		}
		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstTexture->PipelineBarrier(m_vkCommandBuffer, dstImageLayout));
	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));

	return true;
}

bool CVKTransferBuffer::TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	ASSERT(pDstTexture);
	ASSERT(m_vkFence);
	ASSERT(m_pMemory);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkCommandBuffer);

	CALL_VK_FUNCTION_RETURN_BOOL(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
	{
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.imageOffset.x = xoffset;
		region.imageOffset.y = yoffset;
		region.imageOffset.z = 0;
		region.imageExtent.width = width;
		region.imageExtent.height = height;
		region.imageExtent.depth = 1;
		region.imageSubresource.aspectMask = pDstTexture->GetImageAspectFlags();
		region.imageSubresource.mipLevel = level;
		region.imageSubresource.baseArrayLayer = face;
		region.imageSubresource.layerCount = 1;

		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstTexture->PipelineBarrier(m_vkCommandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL));
		{
			vkCmdCopyBufferToImage(m_vkCommandBuffer, m_vkBuffer, pDstTexture->GetImage(), pDstTexture->GetImageLayout(), 1, &region);
		}
		CALL_BOOL_FUNCTION_RETURN_BOOL(pDstTexture->PipelineBarrier(m_vkCommandBuffer, dstImageLayout));
	}
	CALL_VK_FUNCTION_RETURN_BOOL(vkEndCommandBuffer(m_vkCommandBuffer));
	CALL_VK_FUNCTION_RETURN_BOOL(vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, m_vkCommandBuffer, m_vkFence));

	return true;
}

bool CVKTransferBuffer::IsTransferFinish(void) const
{
	ASSERT(m_vkFence);
	return vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence) == VK_SUCCESS;
}
