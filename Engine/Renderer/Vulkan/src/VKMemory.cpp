#include "VKRenderer.h"


mem_node::mem_node(CVKMemory* pFreeMemory)
{
	pMemory = pFreeMemory;
	pMemory->pMemoryNode = this;
}

VkDeviceSize mem_node::size(void)
{
	return pMemory->GetSize();
}

VkDeviceSize mem_node::offset(void)
{
	return pMemory->GetOffset();
}


CVKMemory::CVKMemory(CVKDevice* pDevice, CVKMemoryAllocator* pAllocator, VkDeviceSize memorySize, VkDeviceSize memoryOffset, VkDeviceSize memoryPadding)
	: m_pDevice(pDevice)
	, m_pAllocator(pAllocator)

	, m_memorySize(memorySize)
	, m_memoryOffset(memoryOffset)
	, m_memoryPadding(memoryPadding)

	, bInUse(false)

	, pNext(nullptr)
	, pPrev(nullptr)

	, pMemoryNode(nullptr)
{

}

CVKMemory::~CVKMemory(void)
{

}

CVKMemoryAllocator* CVKMemory::GetAllocator(void) const
{
	return m_pAllocator;
}

VkDeviceSize CVKMemory::GetSize(void) const
{
	return m_memorySize;
}

VkDeviceSize CVKMemory::GetOffset(void) const
{
	return m_memoryOffset;
}

bool CVKMemory::BindImage(VkImage vkImage) const
{
	ASSERT(vkImage);

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), vkImage, &requirements);

	ASSERT(m_memorySize >= requirements.size);
	ASSERT(m_memoryOffset == ALIGN_BYTE(m_memoryOffset, requirements.alignment));

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindImageMemory(m_pDevice->GetDevice(), vkImage, m_pAllocator->GetMemory(), m_memoryOffset));
	return true;
}

bool CVKMemory::BindBuffer(VkBuffer vkBuffer) const
{
	ASSERT(vkBuffer);

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);

	ASSERT(m_memorySize >= requirements.size);
	ASSERT(m_memoryOffset == ALIGN_BYTE(m_memoryOffset, requirements.alignment));

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindBufferMemory(m_pDevice->GetDevice(), vkBuffer, m_pAllocator->GetMemory(), m_memoryOffset));
	return true;
}

bool CVKMemory::BeginMap(void)
{
	return IsHostVisible();
}

bool CVKMemory::CopyData(VkDeviceSize offset, VkDeviceSize size, const void* data)
{
	if (IsHostVisible() == false) {
		return false;
	}

	ASSERT(data);
	ASSERT(size);
	ASSERT(m_memorySize >= offset + size);

	memcpy((uint8_t*)m_pAllocator->GetMemoryAddress() + m_memoryOffset + offset, data, size);
	return true;
}

bool CVKMemory::EndMap(void)
{
	if (IsHostVisible() == false) {
		return false;
	}

	VkMappedMemoryRange range = {};
	range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext = nullptr;
	range.memory = m_pAllocator->GetMemory();
	range.offset = m_memoryOffset;
	range.size = m_memorySize;
	vkFlushMappedMemoryRanges(m_pDevice->GetDevice(), 1, &range);

	return true;
}

bool CVKMemory::IsDeviceLocal(void) const
{
	return m_pAllocator->IsDeviceLocal();
}

bool CVKMemory::IsHostVisible(void) const
{
	return m_pAllocator->IsHostVisible();
}

bool CVKMemory::IsHostCoherent(void) const
{
	return m_pAllocator->IsHostCoherent();
}

bool CVKMemory::IsHostCached(void) const
{
	return m_pAllocator->IsHostCached();
}

bool CVKMemory::IsLazilyAllocated(void) const
{
	return m_pAllocator->IsLazilyAllocated();
}
