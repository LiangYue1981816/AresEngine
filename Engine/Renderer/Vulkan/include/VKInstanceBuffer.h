#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CGfxInstanceBuffer
{
public:
	CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding);
	virtual ~CVKInstanceBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;
	VkDeviceSize GetBufferSize(void) const;

public:
	uint32_t GetBinding(void) const;
	uint32_t GetFormat(void) const;
	uint32_t GetCount(void) const;

public:
	bool CopyDataToDevice(size_t size, const void* data);


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};
