#pragma once
#include "VKRenderer.h"


class CVKTransferBuffer
{
	friend class CVKBuffer;
	friend class CVKTexture;
	friend class CVKTransferManager;


private:
	CVKTransferBuffer(CVKDevice* pDevice, VkQueue vkQueue, VkCommandPool vkCommandPool, VkDeviceSize size);
	virtual ~CVKTransferBuffer(void);


private:
	bool Create(VkQueue vkQueue, VkCommandPool vkCommandPool, VkDeviceSize size);
	void Destroy(void);

private:
	VkDeviceSize GetSize(void) const;
	bool IsTransferFinish(void) const;

private:
	bool TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data);
	bool TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	VkBuffer m_vkBuffer;

private:
	VkQueue m_vkQueue;
	VkFence m_vkFence;

	VkCommandPool m_vkCommandPool;
	VkCommandBuffer m_vkCommandBuffer;

private:
	CVKMemory* m_pMemory;

private:
	CVKDevice* m_pDevice;
};
