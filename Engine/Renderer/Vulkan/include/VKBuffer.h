#pragma once
#include "VKRenderer.h"


class CVKBuffer : public CGfxResource
{
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
	virtual void Release(void) {}


private:
	VkBuffer GetBuffer(void) const;
	VkDeviceSize GetSize(void) const;
	VkDeviceSize GetMemorySize(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;

private:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;

private:
	bool BufferData(size_t offset, size_t size, const void* data);


private:
	VkBuffer m_vkBuffer;
	VkDeviceSize m_vkSize;
	VkBufferUsageFlags m_vkBufferUsageFlags;

private:
	CVKMemory* m_pMemory;

private:
	CVKDevice* m_pDevice;
};
