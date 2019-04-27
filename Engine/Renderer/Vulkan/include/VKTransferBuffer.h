#pragma once
#include "VKRenderer.h"


class CVKTransferBuffer
{
	friend class CVKBuffer;
	friend class CVKTexture;


private:
	CVKTransferBuffer(CVKDevice* pDevice, VkCommandPool vkCommandPool, VkDeviceSize size);
	virtual ~CVKTransferBuffer(void);


private:
	VkDeviceSize GetSize(void) const;
	bool IsTransferFinish(void) const;

private:
	bool TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data);

	bool TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DDataCompressed(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DArrayDataCompressed(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTextureCubemapDataCompressed(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	VkBuffer m_vkBuffer;
	CVKMemory* m_pMemory;

private:
	VkFence m_vkFence;
	VkCommandBuffer m_vkCommandBuffer;
	VkCommandPool m_vkCommandPool;

private:
	CVKDevice* m_pDevice;
};
