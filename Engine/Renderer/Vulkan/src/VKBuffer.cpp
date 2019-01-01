#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice *pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
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
	CALL_VK_FUNCTION_RETURN(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
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

	void *pAddress = nullptr;
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(offset, size, &pAddress));
	{
		memcpy(pAddress, pBuffer, size);
	}
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->Flush(offset, size));
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

	return true;
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	return m_vkBuffer;
}

bool CVKBuffer::IsDeviceLocal(void) const
{
	return m_pMemory ? m_pMemory->IsDeviceLocal() : false;
}

bool CVKBuffer::IsHostVisible(void) const
{
	return m_pMemory ? m_pMemory->IsHostVisible() : false;
}

bool CVKBuffer::IsHostCoherent(void) const
{
	return m_pMemory ? m_pMemory->IsHostCoherent() : false;
}

bool CVKBuffer::IsHostCached(void) const
{
	return m_pMemory ? m_pMemory->IsHostCached() : false;
}

bool CVKBuffer::IsLazilyAllocated(void) const
{
	return m_pMemory ? m_pMemory->IsLazilyAllocated() : false;
}
