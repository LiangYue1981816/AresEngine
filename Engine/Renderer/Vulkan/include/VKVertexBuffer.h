#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CGfxVertexBuffer
{
public:
	CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetVertexBinding(void) const;
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

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};


class CVKMultiVertexBuffer : public CGfxMultiVertexBuffer
{
public:
	CVKMultiVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, int count);
	virtual ~CVKMultiVertexBuffer(void);
	virtual void Release(void);


public:
	bool SetIndex(int index);

public:
	CVKVertexBuffer* GetBuffer(void) const;
	CVKVertexBuffer* GetBuffer(int index) const;

public:
	uint32_t GetVertexBinding(void) const;
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	int m_index;
	eastl::vector<CVKVertexBuffer*> m_pBuffers;
};
