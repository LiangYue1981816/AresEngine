#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CGfxVertexBuffer
{
private:
	CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);


public:
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;
	uint32_t m_size;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
