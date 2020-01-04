#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
public:
	CVKIndirectBuffer(CVKDevice* pDevice, int numDrawCommands);
	virtual ~CVKIndirectBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;
	uint32_t GetStride(void) const;

public:
	bool BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount);


private:
	uint32_t m_size;
	eastl::vector<DrawCommand> m_draws;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};


class CVKMultiIndirectBuffer : public CGfxMultiIndirectBuffer
{
public:
	CVKMultiIndirectBuffer(CVKDevice* pDevice, int numDrawCommands, int count);
	virtual ~CVKMultiIndirectBuffer(void);
	virtual void Release(void);


public:
	void SetIndex(int index);

public:
	CVKIndirectBuffer* GetBuffer(void) const;
	CVKIndirectBuffer* GetBuffer(int index) const;

public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;
	uint32_t GetStride(void) const;

public:
	bool BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount);


private:
	int m_index;
	eastl::vector<CVKIndirectBuffer*> m_pBuffers;
};
