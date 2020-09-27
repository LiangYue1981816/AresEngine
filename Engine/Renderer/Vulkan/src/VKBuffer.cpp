#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice* pDevice, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, GfxBufferType type)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkBufferSize(0)
	, m_vkBufferUsageFlags(0)
	, m_type(GFX_BUFFER_INVALID_ENUM)
{
	m_type = type;

	m_vkBufferSize = bufferSize;
	m_vkBufferUsageFlags = bufferUsageFlags;

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = bufferSize;
	createInfo.usage = bufferUsageFlags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	CALL_VK_FUNCTION_ASSERT(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);

	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, memoryPropertyFlags, VK_RESOURCE_TYPE_BUFFER);

	if (m_pMemory) {
		m_pMemory->BindBuffer(m_vkBuffer);
		CGfxProfiler::IncBufferSize(m_type, m_pMemory->GetSize());
	}
}

CVKBuffer::~CVKBuffer(void)
{
	if (m_vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		CGfxProfiler::DecBufferSize(m_type, m_pMemory->GetSize());
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	return m_vkBuffer;
}

VkBufferUsageFlags CVKBuffer::GetBufferUsageFlags(void) const
{
	return m_vkBufferUsageFlags;
}

VkDeviceSize CVKBuffer::GetBufferSize(void) const
{
	return m_vkBufferSize;
}

VkDeviceSize CVKBuffer::GetMemorySize(void) const
{
	if (m_pMemory) {
		return m_pMemory->GetSize();
	}
	else {
		return 0;
	}
}

bool CVKBuffer::IsDeviceLocal(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsDeviceLocal();
	}
	else {
		return false;
	}
}

bool CVKBuffer::IsHostVisible(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsHostVisible();
	}
	else {
		return false;
	}
}

bool CVKBuffer::IsHostCoherent(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsHostCoherent();
	}
	else {
		return false;
	}
}

bool CVKBuffer::IsHostCached(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsHostCached();
	}
	else {
		return false;
	}
}

bool CVKBuffer::IsLazilyAllocated(void) const
{
	if (m_pMemory) {
		return m_pMemory->IsLazilyAllocated();
	}
	else {
		return false;
	}
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	if (size && data) {
		if (m_pMemory && m_pMemory->IsHostVisible()) {
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(offset, size, data));
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());
			return true;
		}
	}

	return false;
}
