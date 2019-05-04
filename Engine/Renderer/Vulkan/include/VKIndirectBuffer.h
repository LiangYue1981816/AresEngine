#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
private:
	CVKIndirectBuffer(CVKDevice* pDevice, int numDrawCommands);
	virtual ~CVKIndirectBuffer(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetSize(void) const;
	uint32_t GetOffset(void) const;
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;

public:
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);


private:
	uint32_t m_size;
	uint32_t m_offset;
	eastl::vector<DrawCommand> m_draws;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
