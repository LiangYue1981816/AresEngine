#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice *pDevice, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size)
	: m_pDevice(pDevice)

	, m_size(size)
	, m_bufferUsageFlags(bufferUsageFlags)
	, m_memoryPropertyFlags(memoryPropertyFlags)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_pMemory(nullptr)
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = m_size;
	createInfo.usage = m_bufferUsageFlags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	CALL_VK_FUNCTION_RETURN(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, m_memoryPropertyFlags);
	m_pMemory->BindBuffer(m_vkBuffer);
}

CVKBuffer::~CVKBuffer(void)
{
	if (m_vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}
}

void CVKBuffer::Release(void)
{
	delete this;
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_pMemory == nullptr) {
		return false;
	}

	if (IsHostVisible() == false) {
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

bool CVKBuffer::IsDeviceLocal(void) const
{
	return m_memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ? true : false;
}

bool CVKBuffer::IsHostVisible(void) const
{
	return m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ? true : false;
}

bool CVKBuffer::IsHostCoherent(void) const
{
	return m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ? true : false;
}

bool CVKBuffer::IsHostCached(void) const
{
	return m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT ? true : false;
}

bool CVKBuffer::IsLazilyAllocated(void) const
{
	return m_memoryPropertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT ? true : false;
}
