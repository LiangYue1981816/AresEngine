#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkSize(0)
	, m_vkBufferUsageFlags(0)
{
	ASSERT(size);
	ASSERT(bufferUsageFlags);
	ASSERT(memoryPropertyFlags);

	m_vkSize = size;
	m_vkBufferUsageFlags = bufferUsageFlags;

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = size;
	createInfo.usage = bufferUsageFlags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer);

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);

	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, memoryPropertyFlags, VK_RESOURCE_TYPE_BUFFER);
	m_pMemory->BindBuffer(m_vkBuffer);
}

CVKBuffer::~CVKBuffer(void)
{
	vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	return m_vkBuffer;
}

VkDeviceSize CVKBuffer::GetSize(void) const
{
	return m_vkSize;
}

VkDeviceSize CVKBuffer::GetMemorySize(void) const
{
	return m_pMemory->GetSize();
}

VkBufferUsageFlags CVKBuffer::GetBufferUsageFlags(void) const
{
	return m_vkBufferUsageFlags;
}

bool CVKBuffer::IsDeviceLocal(void) const
{
	return m_pMemory->IsDeviceLocal();
}

bool CVKBuffer::IsHostVisible(void) const
{
	return m_pMemory->IsHostVisible();
}

bool CVKBuffer::IsHostCoherent(void) const
{
	return m_pMemory->IsHostCoherent();
}

bool CVKBuffer::IsHostCached(void) const
{
	return m_pMemory->IsHostCached();
}

bool CVKBuffer::IsLazilyAllocated(void) const
{
	return m_pMemory->IsLazilyAllocated();
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	if (size && data) {
		if (m_pMemory->IsHostVisible()) {
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(offset, size, data));
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());
			return true;
		}
	}

	return false;
}
