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
	VkBuffer vkBuffer = VK_NULL_HANDLE;
	CVKMemory *pMemory = nullptr;

	do {
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
		CALL_VK_FUNCTION_BREAK(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &vkBuffer));

		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);
		pMemory = m_pDevice->GetMemoryManager()->AllocMemory(size, requirements.alignment, requirements.memoryTypeBits, m_memoryPropertyFlags);
		CALL_BOOL_FUNCTION_BREAK(pMemory->BindBuffer(vkBuffer));

		void *pAddress = nullptr;
		CALL_BOOL_FUNCTION_BREAK(pMemory->BeginMap(0, size, &pAddress));
		{
			memcpy(pAddress, pBuffer, (size_t)size);
		}
		CALL_BOOL_FUNCTION_BREAK(pMemory->Flush(0, size));
		CALL_BOOL_FUNCTION_BREAK(pMemory->EndMap());

		// ...

		rcode = true;
	} while (true);

	if (vkCommandBuffer) {

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
