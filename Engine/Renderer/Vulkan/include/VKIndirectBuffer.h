#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
	friend class CVKMeshDraw;


private:
	CVKIndirectBuffer(CVKDevice* pDevice, int numDrawCommands);
	virtual ~CVKIndirectBuffer(void);


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
