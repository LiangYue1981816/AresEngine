#include "VKRenderer.h"


// Windows
// DEVICE_LOCAL_BIT 93.5%
// HOST_VISIBLE_BIT | HOST_COHERENT_BIT 89.2%
// HOST_VISIBLE_BIT | HOST_COHERENT_BIT | HOST_CACHED_BIT 89.5%

// Android
// DEVICE_LOCAL_BIT 71.3%
// DEVICE_LOCAL_BIT | HOST_VISIBLE_BIT | HOST_CACHED_BIT 89.4%
// DEVICE_LOCAL_BIT | HOST_VISIBLE_BIT | HOST_COHERENT_BIT 83.5%
// DEVICE_LOCAL_BIT | LAZILY_ALLOCATED_BIT 26.4%

static uint32_t GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize memorySize)
{
	uint32_t indexTry = 0;

	do {
		for (uint32_t indexMemoryType = 0; indexMemoryType < memoryProperties.memoryTypeCount; indexMemoryType++) {
			if ((memoryProperties.memoryTypes[indexMemoryType].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags &&
				(memoryProperties.memoryHeaps[memoryProperties.memoryTypes[indexMemoryType].heapIndex].size * 80 / 100) > memorySize) {
				return indexMemoryType;
			}
		}

		switch (indexTry) {
		case 0:
			memoryPropertyFlags &= ~VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
			break;

		case 1:
			memoryPropertyFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			break;

		case 2:
			memoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			break;

		case 3:
			memoryPropertyFlags &= ~VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		default:
			return INVALID_VALUE;
		}

		indexTry++;
	} while (true);
}


CVKMemoryManager::CVKMemoryManager(CVKDevice* pDevice, VkDeviceSize vkHostMemorySize, VkDeviceSize vkHostOrCacheMemorySize, VkDeviceSize vkHostOrCacheOrDeviceMemorySize, VkDeviceSize vkDeviceMemorySize, VkDeviceSize vkDeviceOrLazilyMemorySize)
	: m_pDevice(pDevice)
{
	pthread_mutex_init(&lock, nullptr);

	// HOST_VISIBLE_BIT
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vkHostMemorySize);
		if (memoryTypeIndex != INVALID_VALUE) {
			if (m_pAllocators.find(memoryTypeIndex) == m_pAllocators.end()) {
				m_pAllocators[memoryTypeIndex] = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, vkHostMemorySize);
			}
		}
	}

	// HOST_VISIBLE_BIT | HOST_CACHED_BIT
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, vkHostOrCacheMemorySize);
		if (memoryTypeIndex != INVALID_VALUE) {
			if (m_pAllocators.find(memoryTypeIndex) == m_pAllocators.end()) {
				m_pAllocators[memoryTypeIndex] = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, vkHostOrCacheMemorySize);
			}
		}
	}

	// HOST_VISIBLE_BIT | HOST_CACHED_BIT | DEVICE_LOCAL_BIT
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkHostOrCacheOrDeviceMemorySize);
		if (memoryTypeIndex != INVALID_VALUE) {
			if (m_pAllocators.find(memoryTypeIndex) == m_pAllocators.end()) {
				m_pAllocators[memoryTypeIndex] = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, vkHostOrCacheOrDeviceMemorySize);
			}
		}
	}

	// DEVICE_LOCAL_BIT
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkDeviceMemorySize);
		if (memoryTypeIndex != INVALID_VALUE) {
			if (m_pAllocators.find(memoryTypeIndex) == m_pAllocators.end()) {
				m_pAllocators[memoryTypeIndex] = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, vkDeviceMemorySize);
			}
		}
	}

	// DEVICE_LOCAL_BIT | LAZILY_ALLOCATED_BIT
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, vkDeviceOrLazilyMemorySize);
		if (memoryTypeIndex != INVALID_VALUE) {
			if (m_pAllocators.find(memoryTypeIndex) == m_pAllocators.end()) {
				m_pAllocators[memoryTypeIndex] = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, vkDeviceOrLazilyMemorySize);
			}
		}
	}
}

CVKMemoryManager::~CVKMemoryManager(void)
{
	for (const auto& itAllocator : m_pAllocators) {
		delete itAllocator.second;
	}

	pthread_mutex_destroy(&lock);
}

CVKMemory* CVKMemoryManager::AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkMemoryPropertyFlags memoryPropertyFlags, VkResourceType type)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryPropertyFlags, 0);
		ASSERT(memoryTypeIndex != INVALID_VALUE);

		if (m_pAllocators.find(memoryTypeIndex) != m_pAllocators.end()) {
			memoryAlignment = ALIGN_BYTE(memoryAlignment, 256);
			memoryAlignment = ALIGN_BYTE(memoryAlignment, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
			memorySize = ALIGN_BYTE(memorySize, memoryAlignment);
			return m_pAllocators[memoryTypeIndex]->AllocMemory(memorySize, memoryAlignment, type);
		}
		else {
			return nullptr;
		}
	}
}

void CVKMemoryManager::FreeMemory(CVKMemory* pMemory)
{
	mutex_autolock autolock(&lock);
	{
		CVKMemoryAllocator* pAllocator = pMemory->GetAllocator();
		pAllocator->FreeMemory(pMemory);
	}
}
