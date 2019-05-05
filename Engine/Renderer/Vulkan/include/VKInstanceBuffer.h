#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CGfxInstanceBuffer
{
private:
	CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding);
	virtual ~CVKInstanceBuffer(void);


public:
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;
	uint32_t m_size;
	uint32_t m_offset;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
