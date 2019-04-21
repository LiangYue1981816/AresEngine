#include "VKRenderer.h"


CVKMemory::CVKMemory(CVKMemoryAllocator* pAllocator, CVKDevice* pDevice, VkDeviceSize memorySize, VkDeviceSize memoryOffset)
	: m_pDevice(pDevice)
	, m_pAllocator(pAllocator)

	, m_memorySize(memorySize)
	, m_memoryOffset(memoryOffset)

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
#ifdef DEBUG
	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), vkImage, &requirements);
	ASSERT(m_memorySize >= requirements.size);
	ASSERT(ALIGN_BYTE(m_memoryOffset, requirements.alignment) == m_memoryOffset);
#endif

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindImageMemory(m_pDevice->GetDevice(), vkImage, m_pAllocator->GetMemory(), m_memoryOffset));
	return true;
}

bool CVKMemory::BindBuffer(VkBuffer vkBuffer) const
{
#ifdef DEBUG
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);
	ASSERT(m_memorySize >= requirements.size);
	ASSERT(ALIGN_BYTE(m_memoryOffset, requirements.alignment) == m_memoryOffset);
#endif

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindBufferMemory(m_pDevice->GetDevice(), vkBuffer, m_pAllocator->GetMemory(), m_memoryOffset));
	return true;
}

bool CVKMemory::BeginMap(VkDeviceSize offset, VkDeviceSize size, void** ppAddress) const
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (m_memorySize < offset + size) {
		return false;
	}

	CALL_VK_FUNCTION_RETURN_BOOL(vkMapMemory(m_pDevice->GetDevice(), m_pAllocator->GetMemory(), m_memoryOffset + offset, size, 0, ppAddress));
	return true;
}

bool CVKMemory::EndMap(void) const
{
	if (IsHostVisible() == false) {
		return false;
	}

	vkUnmapMemory(m_pDevice->GetDevice(), m_pAllocator->GetMemory());
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

	if (m_memorySize < offset + size) {
		return false;
	}

	VkMappedMemoryRange memoryRange = {};
	memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memoryRange.pNext = nullptr;
	memoryRange.memory = m_pAllocator->GetMemory();
	memoryRange.offset = m_memoryOffset + offset;
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

	if (m_memorySize < offset + size) {
		return false;
	}

	VkMappedMemoryRange memoryRange = {};
	memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memoryRange.pNext = nullptr;
	memoryRange.memory = m_pAllocator->GetMemory();
	memoryRange.offset = m_memoryOffset + offset;
	memoryRange.size = size;
	CALL_VK_FUNCTION_RETURN_BOOL(vkInvalidateMappedMemoryRanges(m_pDevice->GetDevice(), 1, &memoryRange));
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
