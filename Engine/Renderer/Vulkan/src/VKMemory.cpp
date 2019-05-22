#include "VKRenderer.h"


CVKMemory::CVKMemory(CVKMemoryAllocator* pAllocator, CVKDevice* pDevice, VkDeviceSize memorySize, VkDeviceSize memoryOffset)
	: m_pDevice(pDevice)
	, m_pAllocator(pAllocator)

	, m_memorySize(memorySize)
	, m_memoryOffset(memoryOffset)

	, m_memoryMapSize(0)
	, m_memoryMapOffset(0)
	, m_memoryMapAddress(nullptr)

	, bInUse(false)
	, pNext(nullptr)
	, pPrev(nullptr)
	, pFreeNext(nullptr)
	, pFreePrev(nullptr)
{
	ASSERT(m_pDevice);
	ASSERT(m_pAllocator);
	ASSERT(m_memorySize);
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

bool CVKMemory::BeginMap(VkDeviceSize offset, VkDeviceSize size)
{
	ASSERT(size);
	ASSERT(m_memorySize >= offset + size);
	ASSERT(m_memoryMapSize == 0);
	ASSERT(m_memoryMapOffset == 0);
	ASSERT(m_memoryMapAddress == nullptr);

	if (IsHostVisible() == false) {
		return false;
	}

	void* address = nullptr;
	CALL_VK_FUNCTION_RETURN_BOOL(vkMapMemory(m_pDevice->GetDevice(), m_pAllocator->GetMemory(), m_memoryOffset + offset, size, 0, &address));

	m_memoryMapSize = size;
	m_memoryMapOffset = offset;
	m_memoryMapAddress = address;

	return true;
}

bool CVKMemory::CopyData(VkDeviceSize offset, VkDeviceSize size, const void* data)
{
	ASSERT(data);
	ASSERT(size);
	ASSERT(m_memoryMapSize >= offset + size);
	ASSERT(m_memoryMapAddress != nullptr);

	if (IsHostVisible() == false) {
		return false;
	}

	memcpy((uint8_t*)m_memoryMapAddress + offset, data, size);
	return true;
}

bool CVKMemory::FlushMappedMemoryRange(VkDeviceSize offset, VkDeviceSize size)
{
	ASSERT(size);
	ASSERT(m_memoryMapSize >= offset + size);
	ASSERT(m_memoryMapAddress != nullptr);

	if (IsHostVisible() == false) {
		return false;
	}

	VkMappedMemoryRange range = {};
	range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext = nullptr;
	range.memory = m_pAllocator->GetMemory();
	range.offset = m_memoryOffset + m_memoryMapOffset + offset;
	range.size = size;

	CALL_VK_FUNCTION_RETURN_BOOL(vkFlushMappedMemoryRanges(m_pDevice->GetDevice(), 1, &range));
	return true;
}

bool CVKMemory::InvalidateMappedMemoryRange(VkDeviceSize offset, VkDeviceSize size)
{
	ASSERT(size);
	ASSERT(m_memoryMapSize >= offset + size);
	ASSERT(m_memoryMapAddress != nullptr);

	if (IsHostVisible() == false) {
		return false;
	}

	VkMappedMemoryRange range = {};
	range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext = nullptr;
	range.memory = m_pAllocator->GetMemory();
	range.offset = m_memoryOffset + m_memoryMapOffset + offset;
	range.size = size;

	CALL_VK_FUNCTION_RETURN_BOOL(vkInvalidateMappedMemoryRanges(m_pDevice->GetDevice(), 1, &range));
	return true;
}

bool CVKMemory::EndMap(void)
{
	ASSERT(m_memoryMapSize);
	ASSERT(m_memoryMapAddress);

	if (IsHostVisible() == false) {
		return false;
	}

	vkUnmapMemory(m_pDevice->GetDevice(), m_pAllocator->GetMemory());

	m_memoryMapSize = 0;
	m_memoryMapOffset = 0;
	m_memoryMapAddress = nullptr;

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
