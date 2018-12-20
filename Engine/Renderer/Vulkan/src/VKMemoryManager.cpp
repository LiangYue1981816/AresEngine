#include "VKRenderer.h"


static int GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties &memoryProperties, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
{
	for (uint32_t indexMemoryType = 0; indexMemoryType < memoryProperties.memoryTypeCount; indexMemoryType++) {
		if ((memoryTypeBits & (1 << indexMemoryType)) &&
			(memoryProperties.memoryTypes[indexMemoryType].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
			return indexMemoryType;
		}
	}

	return -1;
}
