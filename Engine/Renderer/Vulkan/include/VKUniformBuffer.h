#pragma once
#include "VKRenderer.h"


class CVKUniformBuffer : public CGfxUniformBuffer
{
	friend class CVKUniformBufferManager;


private:
	CVKUniformBuffer(CVKDevice* pDevice, CVKUniformBufferManager* pManager, size_t size);
	virtual ~CVKUniformBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;
	VkDeviceSize GetBufferSize(void) const;
	VkDeviceSize GetMemorySize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);


private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;

private:
	CVKUniformBufferManager* m_pManager;
};
