#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice *pDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags)
{

}

CVKBuffer::~CVKBuffer(void)
{

}


bool CVKBuffer::BufferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
{
	return true;
}

bool CVKBuffer::CopyData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
{
	return true;
}

bool CVKBuffer::TransferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
{
	return true;
}
