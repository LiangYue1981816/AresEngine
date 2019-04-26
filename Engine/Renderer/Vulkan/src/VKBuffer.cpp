#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
	: m_pDevice(pDevice)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_pMemory(nullptr)
{
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
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
	m_pMemory->BindBuffer(m_vkBuffer);
}

CVKBuffer::~CVKBuffer(void)
{
	vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
}

void CVKBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	return m_vkBuffer;
}

VkDeviceSize CVKBuffer::GetSize(void) const
{
	return m_pMemory->GetSize();
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	if (m_pMemory->IsHostVisible()) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(offset, size));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());
	}
	else {
		m_transferRegions.insert(m_transferRegions.end(), VkBufferCopy{ m_transferBuffers.size(), offset, size });
		m_transferBuffers.insert(m_transferBuffers.end(), (uint8_t*)data, (uint8_t*)data + size);
	}

	return true;
}
