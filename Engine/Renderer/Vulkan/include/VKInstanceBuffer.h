#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CGfxInstanceBuffer
{
public:
	CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding);
	virtual ~CVKInstanceBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetInstanceBinding(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;
	uint32_t m_size;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};


class CVKMultiInstanceBuffer : public CGfxMultiInstanceBuffer
{
public:
	CVKMultiInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding, int count);
	virtual ~CVKMultiInstanceBuffer(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	uint32_t GetInstanceBinding(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	int m_index;
	eastl::vector<CVKInstanceBuffer*> m_pBuffers;
};
