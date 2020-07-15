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
	friend class CVKStorageBuffer;
	friend class CVKTransferBuffer;


private:
	CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
	virtual ~CVKBuffer(void);
	virtual void Release(void);


private:
	bool Create(VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
	void Destroy(void);

private:
	VkBuffer GetBuffer(void) const;
	VkDeviceSize GetSize(void) const;
	VkDeviceSize GetRange(void) const;
	VkDeviceSize GetOffset(void) const;
	VkDeviceSize GetMemorySize(void) const;

private:
	bool BufferData(size_t offset, size_t size, const void* data);
	bool BufferRange(size_t offset, size_t range);

private:
	bool PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkDeviceSize offset, VkDeviceSize size);
	bool PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size);

private:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;


private:
	VkBuffer m_vkBuffer;
	VkDeviceSize m_vkSize;
	VkDeviceSize m_vkRange;
	VkDeviceSize m_vkOffset;
	VkBufferUsageFlags m_vkBufferUsageFlags;

private:
	CVKMemory* m_pMemory;

private:
	CVKDevice* m_pDevice;
};
