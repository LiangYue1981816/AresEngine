#include "VKRenderer.h"


CVKMemory::CVKMemory(CVKMemoryAllocator *pAllocator, CVKDevice *pDevice, VkDeviceMemory vkMemory, VkMemoryPropertyFlags flags, VkDeviceSize offset, VkDeviceSize aligmentOffset, VkDeviceSize size)
	: m_pDevice(pDevice)

	, m_pAllocator(pAllocator)
	, m_vkMemory(vkMemory)

	, m_flags(flags)
	, m_offset(offset)
	, m_aligmentOffset(aligmentOffset)
	, m_size(size)

	, bInUse(false)

	, pNext(nullptr)
	, pPrev(nullptr)
	, pFreeNext(nullptr)
	, pFreePrev(nullptr)
{

}

CVKMemory::~CVKMemory(void)
{

}

CVKMemoryAllocator* CVKMemory::GetAllocator(void) const
{
	return m_pAllocator;
}

bool CVKMemory::BindImage(VkImage vkImage) const
{
#ifdef DEBUG
	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), vkImage, &requirements);
	ASSERT(m_size >= requirements.size);
	ASSERT(ALIGN_BYTE(m_size, requirements.alignment) == m_size);
	ASSERT(ALIGN_BYTE(m_offset + m_aligmentOffset, requirements.alignment) == m_offset + m_aligmentOffset);
#endif
	CALL_VK_FUNCTION_RETURN_BOOL(vkBindImageMemory(m_pDevice->GetDevice(), vkImage, m_vkMemory, m_offset + m_aligmentOffset));
	return true;
}

bool CVKMemory::BindBuffer(VkBuffer vkBuffer) const
{
#ifdef DEBUG
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);
	ASSERT(m_size >= requirements.size);
	ASSERT(ALIGN_BYTE(m_size, requirements.alignment) == m_size);
	ASSERT(ALIGN_BYTE(m_offset + m_aligmentOffset, requirements.alignment) == m_offset + m_aligmentOffset);
#endif
	CALL_VK_FUNCTION_RETURN_BOOL(vkBindBufferMemory(m_pDevice->GetDevice(), vkBuffer, m_vkMemory, m_offset + m_aligmentOffset));
	return true;
}

bool CVKMemory::BeginMap(VkDeviceSize offset, VkDeviceSize size, void **ppAddress) const
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (m_size < offset + size) {
		return false;
	}

	CALL_VK_FUNCTION_RETURN_BOOL(vkMapMemory(m_pDevice->GetDevice(), m_vkMemory, m_offset + m_aligmentOffset + offset, size, m_flags, ppAddress));
	return true;
}

bool CVKMemory::EndMap(void) const
{
	if (IsHostVisible() == false) {
		return false;
	}

	vkUnmapMemory(m_pDevice->GetDevice(), m_vkMemory);
	return true;
}

bool CVKMemory::Flush(VkDeviceSize offset, VkDeviceSize size) const
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (IsHostCoherent()) {
		return true;
	}

	if (m_size < offset + size) {
		return false;
	}

	VkMappedMemoryRange memoryRange = {};
	memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memoryRange.pNext = nullptr;
	memoryRange.memory = m_vkMemory;
	memoryRange.offset = m_offset + m_aligmentOffset + offset;
	memoryRange.size = size;
	CALL_VK_FUNCTION_RETURN_BOOL(vkFlushMappedMemoryRanges(m_pDevice->GetDevice(), 1, &memoryRange));
	return true;
}

bool CVKMemory::Invalidate(VkDeviceSize offset, VkDeviceSize size) const
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (IsHostCoherent()) {
		return true;
	}

	if (m_size < offset + size) {
		return false;
	}

	VkMappedMemoryRange memoryRange = {};
	memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memoryRange.pNext = nullptr;
	memoryRange.memory = m_vkMemory;
	memoryRange.offset = m_offset + m_aligmentOffset + offset;
	memoryRange.size = size;
	CALL_VK_FUNCTION_RETURN_BOOL(vkInvalidateMappedMemoryRanges(m_pDevice->GetDevice(), 1, &memoryRange));
	return true;
}

bool CVKMemory::IsDeviceLocal(void) const
{
	return m_flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ? true : false;
}

bool CVKMemory::IsHostVisible(void) const
{
	return m_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ? true : false;
}

bool CVKMemory::IsHostCoherent(void) const
{
	return m_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ? true : false;
}

bool CVKMemory::IsHostCached(void) const
{
	return m_flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT ? true : false;
}

bool CVKMemory::IsLazilyAllocated(void) const
{
	return m_flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT ? true : false;
}

VkDeviceSize CVKMemory::GetOffset(void) const
{
	return m_offset;
}

VkDeviceSize CVKMemory::GetAligmentOffset(void) const
{
	return m_aligmentOffset;
}

VkDeviceSize CVKMemory::GetSize(void) const
{
	return m_size;
}
