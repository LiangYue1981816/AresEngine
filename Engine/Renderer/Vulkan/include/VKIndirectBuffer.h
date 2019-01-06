#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
	friend class CVKDevice;
	friend class CVKMeshDraw;


private:
	CVKIndirectBuffer(CVKDevice *pDevice, uint32_t drawCommandCount);
	virtual ~CVKIndirectBuffer(void);


public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);


private:
	uint32_t m_size;
	uint32_t m_count;

private:
	eastl::vector<DrawCommand> m_draws;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
