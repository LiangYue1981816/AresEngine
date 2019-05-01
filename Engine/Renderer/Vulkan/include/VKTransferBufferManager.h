#pragma once
#include "VKRenderer.h"


class CVKTransferBufferManager
{
	friend class CVKDevice;
	friend class CVKTransferBuffer;


private:
	CVKTransferBufferManager(CVKDevice* pDevice, uint32_t queueFamilyIndex);
	virtual ~CVKTransferBufferManager(void);


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
	eastl::vector<CVKTransferBuffer*> m_pendingList;
	eastl::unordered_map<VkDeviceSize, eastl::list<CVKTransferBuffer*>> m_freeLists;

private:
	CVKDevice* m_pDevice;
};
