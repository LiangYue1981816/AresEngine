#pragma once
#include "VKRenderer.h"


class CVKTransferManager
{
	friend class CVKDevice;
	friend class CVKTransferBuffer;


private:
	CVKTransferManager(CVKDevice* pDevice, uint32_t queueFamilyIndex);
	virtual ~CVKTransferManager(void);


private:
	CVKTransferBuffer* AcquireTransferBuffer(size_t size);

public:
	bool TransferBufferData(CVKBuffer* pDstBuffer, VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstPipelineStageFlags, size_t offset, size_t size, const void* data);
	bool TransferTexture2DData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DArrayData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTextureCubemapData(CVKTexture* pDstTexture, VkImageLayout dstImageLayout, int face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	VkQueue m_vkQueue;
	VkCommandPool m_vkCommandPool;

private:
	pthread_mutex_t lock;
	eastl::unordered_map<VkDeviceSize, CVKMemory*> m_pMemorys;
	eastl::unordered_map<VkDeviceSize, CVKTransferBuffer*> m_pTransferBuffers;

private:
	CVKDevice* m_pDevice;
};
