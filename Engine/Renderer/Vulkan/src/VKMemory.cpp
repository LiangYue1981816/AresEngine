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
	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(m_pDevice->GetDevice(), vkImage, &requirements);

	if (m_memorySize < requirements.size) {
		return false;
	}

	if ((ALIGN_BYTE(m_memoryOffset, requirements.alignment) != m_memoryOffset)) {
		return false;
	}

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindImageMemory(m_pDevice->GetDevice(), vkImage, m_pAllocator->GetMemory(), m_memoryOffset));
	return true;
}

bool CVKMemory::BindBuffer(VkBuffer vkBuffer) const
{
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);

	if (m_memorySize < requirements.size) {
		return false;
	}

	if ((ALIGN_BYTE(m_memoryOffset, requirements.alignment) != m_memoryOffset)) {
		return false;
	}

	CALL_VK_FUNCTION_RETURN_BOOL(vkBindBufferMemory(m_pDevice->GetDevice(), vkBuffer, m_pAllocator->GetMemory(), m_memoryOffset));
	return true;
}

bool CVKMemory::BeginMap(VkDeviceSize offset, VkDeviceSize size)
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (IsHostCoherent() == false) {
		return false;
	}

	if (m_memoryMapAddress != nullptr) {
		return false;
	}

	if (m_memorySize < offset + size) {
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
	if (IsHostVisible() == false) {
		return false;
	}

	if (IsHostCoherent() == false) {
		return false;
	}

	if (m_memoryMapAddress == nullptr) {
		return false;
	}

	if (m_memoryMapSize < offset + size) {
		return false;
	}

	memcpy((uint8_t*)m_memoryMapAddress + offset, data, size);
	return true;
}

bool CVKMemory::EndMap(void)
{
	if (IsHostVisible() == false) {
		return false;
	}

	if (IsHostCoherent() == false) {
		return false;
	}

	if (m_memoryMapAddress == nullptr) {
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
