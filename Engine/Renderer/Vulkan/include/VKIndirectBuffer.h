#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
	friend class CVKDevice;


private:
	CVKIndirectBuffer(CVKDevice *pDevice, uint32_t count);
	virtual ~CVKIndirectBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void *pParam);


private:
	eastl::vector<DrawCommand> m_draws;

	
private:
	VkBuffer m_vkBuffer;
	VkBufferUsageFlags m_usage;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

private:
	CVKDevice *m_pDevice;
};
