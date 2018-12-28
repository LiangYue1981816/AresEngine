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
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);

private:
	CVKBufferPtr GetBuffer(void) const;


private:
	eastl::vector<DrawCommand> m_draws;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
