#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
	, m_memoryPropertyFlags(bDynamic ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)

	, m_pMemory(nullptr)
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = bDynamic ? CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * size : size;
	createInfo.usage = m_usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	CALL_VK_FUNCTION_RETURN(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(size, requirements.alignment, requirements.memoryTypeBits, m_memoryPropertyFlags);
	CALL_BOOL_FUNCTION_RETURN(m_pMemory->BindBuffer(m_vkBuffer));
}

CVKVertexBuffer::~CVKVertexBuffer(void)
{
	if (m_vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}
}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
		return CopyData(offset, size, pBuffer);
	}
	else {
		return TransferData(offset, size, pBuffer);
	}
}

bool CVKVertexBuffer::CopyData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	void *pAddress = nullptr;
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(VKRenderer()->GetSwapChain()->GetTextureIndex() * m_size + offset, size, &pAddress));
	{
		memcpy(pAddress, pBuffer, (size_t)size);
	}
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->Flush(VKRenderer()->GetSwapChain()->GetTextureIndex() * m_size + offset, size));
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

	return true;
}

bool CVKVertexBuffer::TransferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	bool rcode = false;

	VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
	VkFence vkFence = VK_NULL_HANDLE;

	VkBuffer vkBuffer = VK_NULL_HANDLE;
	CVKMemory *pMemory = nullptr;

	do {
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.pNext = nullptr;
		bufferCreateInfo.flags = 0;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = 0;
		bufferCreateInfo.pQueueFamilyIndices = nullptr;
		CALL_VK_FUNCTION_BREAK(vkCreateBuffer(m_pDevice->GetDevice(), &bufferCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &vkBuffer));

		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);
		pMemory = m_pDevice->GetMemoryManager()->AllocMemory(size, requirements.alignment, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		CALL_BOOL_FUNCTION_BREAK(pMemory->BindBuffer(vkBuffer));

		void *pAddress = nullptr;
		CALL_BOOL_FUNCTION_BREAK(pMemory->BeginMap(0, size, &pAddress));
		{
			memcpy(pAddress, pBuffer, (size_t)size);
		}
		CALL_BOOL_FUNCTION_BREAK(pMemory->Flush(0, size));
		CALL_BOOL_FUNCTION_BREAK(pMemory->EndMap());

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		CALL_VK_FUNCTION_BREAK(vkCreateFence(m_pDevice->GetDevice(), &fenceCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &vkFence));

		CALL_BOOL_FUNCTION_BREAK(m_pDevice->GetTransferQueue()->AllocCommandBuffer(0, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &vkCommandBuffer));
		CALL_VK_FUNCTION_BREAK(vkBeginCommandBufferPrimary(vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
		{
			vkCmdTransferBuffer(vkCommandBuffer, vkBuffer, m_vkBuffer, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, offset, size);
		}
		CALL_VK_FUNCTION_BREAK(vkEndCommandBuffer(vkCommandBuffer));
		CALL_VK_FUNCTION_BREAK(vkSubmitCommandBuffer(m_pDevice->GetTransferQueue()->GetQueue(), vkCommandBuffer, vkFence));
		CALL_VK_FUNCTION_BREAK(vkWaitForFences(m_pDevice->GetDevice(), 1, &vkFence, VK_FALSE, 0xffffffff));

		rcode = true;
		break;
	} while (true);

	if (vkCommandBuffer) {
		m_pDevice->GetTransferQueue()->FreeCommandBuffer(0, vkCommandBuffer);
	}

	if (vkFence) {
		vkDestroyFence(m_pDevice->GetDevice(), vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(pMemory);
	}

	return rcode;
}

void CVKVertexBuffer::Bind(void *pParam)
{

}
