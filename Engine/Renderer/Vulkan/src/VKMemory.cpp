#include "VKRenderer.h"


CVKMemory::CVKMemory(CVKDevice* pDevice, CVKMemoryAllocator* pAllocator, VkDeviceSize memorySize, VkDeviceSize memoryOffset)
	: m_pDevice(pDevice)
	, m_pAllocator(pAllocator)

	, m_memorySize(memorySize)
	, m_memoryOffset(memoryOffset)
	, m_memoryPadding(0)

	, type(VK_RESOURCE_TYPE_FREE)

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
	return m_memorySize - m_memoryPadding;
}

VkDeviceSize CVKMemory::GetOffset(void) const
{
	return m_memoryOffset + m_memoryPadding;
}

bool CVKMemory::BindImage(VkImage vkImage) const
{
	ASSERT(vkImage);

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), vkImage, &requirements);

	ASSERT(GetSize() >= requirements.size);
	ASSERT(GetOffset() == ALIGN_BYTE(GetOffset(), requirements.alignment));

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindImageMemory(m_pDevice->GetDevice(), vkImage, m_pAllocator->GetMemory(), GetOffset()));
	return true;
}

bool CVKMemory::BindBuffer(VkBuffer vkBuffer) const
{
	ASSERT(vkBuffer);

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);

	ASSERT(GetSize() >= requirements.size);
	ASSERT(GetOffset() == ALIGN_BYTE(GetOffset(), requirements.alignment));

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindBufferMemory(m_pDevice->GetDevice(), vkBuffer, m_pAllocator->GetMemory(), GetOffset()));
	return true;
}

bool CVKMemory::BeginMap(bool bInvalidate)
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (bInvalidate) {
		VkMappedMemoryRange range = {};
		range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		range.pNext = nullptr;
		range.memory = m_pAllocator->GetMemory();
		range.offset = GetOffset();
		range.size = GetSize();
		CALL_VK_FUNCTION_RETURN_BOOL(vkInvalidateMappedMemoryRanges(m_pDevice->GetDevice(), 1, &range));
	}

	return true;
}

bool CVKMemory::CopyDataToDevice(VkDeviceSize offset, VkDeviceSize size, const void* data)
{
	ASSERT(data);
	ASSERT(size);

	if (IsHostVisible() == false) {
		return false;
	}

	if (GetSize() < offset + size) {
		return false;
	}

	memcpy((uint8_t*)m_pAllocator->GetMemoryAddress() + GetOffset() + offset, data, size);
	return true;
}

bool CVKMemory::CopyDataToHost(VkDeviceSize offset, VkDeviceSize size, void* data)
{
	ASSERT(data);
	ASSERT(size);

	if (IsHostVisible() == false) {
		return false;
	}

	if (GetSize() < offset + size) {
		return false;
	}

	memcpy(data, (uint8_t*)m_pAllocator->GetMemoryAddress() + GetOffset() + offset, size);
	return true;
}

bool CVKMemory::EndMap(bool bFlush)
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (bFlush) {
		VkMappedMemoryRange range = {};
		range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		range.pNext = nullptr;
		range.memory = m_pAllocator->GetMemory();
		range.offset = GetOffset();
		range.size = GetSize();
		CALL_VK_FUNCTION_RETURN_BOOL(vkFlushMappedMemoryRanges(m_pDevice->GetDevice(), 1, &range));
	}

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
