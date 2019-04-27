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
	bool TransferBufferData(CVKBuffer* pTransferDstBuffer, size_t offset, size_t size, const void* data) const;

	bool TransferTexture2DData(CVKTexture* pTransferDstTexture, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DDataCompressed(CVKTexture* pTransferDstTexture, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTexture2DArrayData(CVKTexture* pTransferDstTexture, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DArrayDataCompressed(CVKTexture* pTransferDstTexture, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTextureCubemapData(CVKTexture* pTransferDstTexture, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTextureCubemapDataCompressed(CVKTexture* pTransferDstTexture, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


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
