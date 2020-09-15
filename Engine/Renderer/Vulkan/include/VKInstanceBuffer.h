#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CGfxInstanceBuffer
{
	friend class CVKInstanceBufferManager;


private:
	CVKInstanceBuffer(CVKDevice* pDevice, CVKInstanceBufferManager* pManager, uint32_t instanceFormat, int instanceBinding);
	virtual ~CVKInstanceBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetInstanceBinding(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer, int offset) const;


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;

private:
	CVKInstanceBufferManager* m_pManager;
};
