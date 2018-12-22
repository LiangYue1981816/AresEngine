#include "VKRenderer.h"


CVKBufferBase::CVKBufferBase(CVKDevice *pDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_usage(0)
	, m_flags(0)
{
	Create(size, usage, memoryPropertyFlags);
}

CVKBufferBase::~CVKBufferBase(void)
{
	Destroy();
}

bool CVKBufferBase::Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags)
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = size;
	createInfo.usage = usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BindBuffer(m_vkBuffer));

	m_usage = usage;
	m_flags = memoryPropertyFlags;

	return true;
}

void CVKBufferBase::Destroy(void)
{
	if (m_vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}

	m_usage = 0;
	m_vkBuffer = VK_NULL_HANDLE;
	m_pMemory = nullptr;
}

bool CVKBufferBase::BufferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
{
	void *pAddress = nullptr;

	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(offset, size, &pAddress));
	{
		memcpy(pAddress, pBuffer, (size_t)size);
	}
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->Flush(offset, size));
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());

	return true;
}
