#pragma once
#include "VKRenderer.h"


class CVKBuffer : public CGfxResource
{
	friend class CVKTexture;
	friend class CVKIndexBuffer;
	friend class CVKVertexBuffer;
	friend class CVKInstanceBuffer;
	friend class CVKIndirectBuffer;
	friend class CVKUniformBuffer;
	friend class CVKTransferBuffer;


private:
	CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
	virtual ~CVKBuffer(void);
	virtual void Release(void);


private:
	VkBuffer GetBuffer(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;
	VkDeviceSize GetSize(void) const;

private:
	bool BufferData(size_t offset, size_t size, const void* data);
	bool PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size);


private:
	VkBuffer m_vkBuffer;
	VkBufferUsageFlags m_vkBufferUsageFlags;
	CVKMemory* m_pMemory;

private:
	CVKDevice* m_pDevice;
};

typedef CGfxResourcePtr<CVKBuffer> CVKBufferPtr;
