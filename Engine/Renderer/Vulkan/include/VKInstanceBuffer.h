#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CGfxInstanceBuffer
{
	friend class CVKMultiInstanceBuffer;
	friend class CVKInstanceBufferManager;


private:
	CVKInstanceBuffer(CVKDevice* pDevice, CVKInstanceBufferManager* pManager, uint32_t instanceFormat, int instanceBinding);
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

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;

private:
	CVKInstanceBufferManager* m_pManager;
};


class CVKMultiInstanceBuffer : public CGfxMultiInstanceBuffer
{
	friend class CVKInstanceBufferManager;


private:
	CVKMultiInstanceBuffer(CVKDevice* pDevice, CVKInstanceBufferManager* pManager, uint32_t instanceFormat, int instanceBinding, int count);
	virtual ~CVKMultiInstanceBuffer(void);
	virtual void Release(void);


public:
	CGfxInstanceBuffer* GetBuffer(int index) const;


private:
	eastl::vector<CVKInstanceBuffer*> m_pBuffers;

private:
	CVKInstanceBufferManager* m_pManager;
};
