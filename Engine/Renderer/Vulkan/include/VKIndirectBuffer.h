#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
private:
	CVKIndirectBuffer(CVKDevice* pDevice, uint32_t numDrawCommands);
	virtual ~CVKIndirectBuffer(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);


private:
	eastl::vector<DrawCommand> m_draws;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
