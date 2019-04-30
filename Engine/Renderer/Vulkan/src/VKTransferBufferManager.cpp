#include "VKRenderer.h"


CVKTransferBufferManager::CVKTransferBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKTransferBufferManager::~CVKTransferBufferManager(void)
{

}

bool CVKTransferBufferManager::TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data)
{
	return true;
}

bool CVKTransferBufferManager::TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBufferManager::TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}

bool CVKTransferBufferManager::TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return true;
}
